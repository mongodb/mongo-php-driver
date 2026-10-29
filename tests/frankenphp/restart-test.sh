#!/usr/bin/env bash
#
# Functional test for PHPC-2797: FrankenPHP reboots all its PHP threads
# in-process when opcache_reset() is called. The engine then runs GSHUTDOWN and
# MINIT again in the same process, while libmongoc can only be initialized once
# per process. Before the fix, the next Manager creation aborted the process in
# mongoc_handshake_data_append().
#
# The worker also runs a command on a MongoDB server on every request, so the
# test checks that the connection still works after the restart.
#
# Requires Docker. Run it with:
#   bash tests/frankenphp/restart-test.sh

set -o errexit
set -o nounset
set -o pipefail

REPO_ROOT=$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)

NETWORK_NAME=${NETWORK_NAME:-mongodb-frankenphp-restart}
MONGO_CONTAINER_NAME=${MONGO_CONTAINER_NAME:-mongodb-frankenphp-mongo}
CONTAINER_NAME=${CONTAINER_NAME:-mongodb-frankenphp-restart}
IMAGE_NAME=${IMAGE_NAME:-mongodb-frankenphp}
PORT=${PORT:-8080}
BASE_URL="http://127.0.0.1:${PORT}"

cleanup() {
    docker logs "${CONTAINER_NAME}" 2>&1 || true
    docker rm --force --volumes "${CONTAINER_NAME}" "${MONGO_CONTAINER_NAME}" >/dev/null 2>&1 || true
    docker network rm "${NETWORK_NAME}" >/dev/null 2>&1 || true
}
trap cleanup EXIT

# Set SKIP_BUILD when the image was already built, for example by the CI
# workflow with a layer cache.
if [ -z "${SKIP_BUILD:-}" ]; then
    docker build --tag "${IMAGE_NAME}" --file "${REPO_ROOT}/tests/frankenphp/Dockerfile" "${REPO_ROOT}"
fi

# Containers on their own network, so FrankenPHP reaches MongoDB by name.
docker network create "${NETWORK_NAME}" >/dev/null
docker run --detach \
    --name "${MONGO_CONTAINER_NAME}" \
    --network "${NETWORK_NAME}" \
    --network-alias mongodb \
    mongo:8.0 >/dev/null

# ZEND_DONT_UNLOAD_MODULES keeps the extension mapped when the engine shuts
# down, like SAPIs that restart the engine in-process. Without it, the extension
# is unloaded and reloaded with fresh global state, which hides PHPC-2797.
docker run --detach \
    --name "${CONTAINER_NAME}" \
    --network "${NETWORK_NAME}" \
    --env ZEND_DONT_UNLOAD_MODULES=1 \
    --env MONGODB_URI="mongodb://mongodb:27017/?serverSelectionTimeoutMS=2000" \
    --publish "${PORT}:80" \
    "${IMAGE_NAME}" >/dev/null

get() {
    curl --silent --fail --max-time 30 "${BASE_URL}$1"
}

# Wait for the worker to answer successful requests, which also waits for
# MongoDB. PHP errors can be returned with a 200 status, so check the body.
echo "Waiting for FrankenPHP..."
body_before=""
for _ in $(seq 1 60); do
    body_before=$(get / || true)
    if [ "${body_before#ok }" != "${body_before}" ]; then
        break
    fi
    sleep 1
done

if [ "${body_before#ok }" = "${body_before}" ]; then
    echo "FAILED: unexpected response before the reboot: ${body_before}"
    exit 1
fi
instance_before=${body_before#ok }
echo "Worker instance before the reboot: ${instance_before}"

# opcache_reset() makes FrankenPHP reboot all its PHP threads in-process.
if [ "$(get /reset)" != "reset" ]; then
    echo "FAILED: opcache_reset() did not run, the PHP threads were not rebooted"
    exit 1
fi

# The worker script runs again in the new engine lifecycle, so it reports a new
# instance identifier. Requests retry while the threads are being rebooted. If
# the extension aborted the process, or the connection to the server is broken,
# the requests fail.
body_after=""
for _ in $(seq 1 30); do
    body_after=$(get / || true)
    if [ "${body_after#ok }" != "${body_after}" ] && [ "${body_after#ok }" != "${instance_before}" ]; then
        break
    fi
    sleep 1
done

if [ "${body_after#ok }" = "${body_after}" ]; then
    echo "FAILED: the worker did not answer or the command failed after the reboot: ${body_after}"
    exit 1
fi

instance_after=${body_after#ok }
if [ "${instance_after}" = "${instance_before}" ]; then
    echo "FAILED: the PHP threads were not rebooted, the worker instance did not change"
    exit 1
fi

echo "Worker instance after the reboot: ${instance_after}"

if [ "$(docker inspect --format '{{.State.Running}}' "${CONTAINER_NAME}")" != "true" ]; then
    echo "FAILED: the FrankenPHP process died after the in-process reboot"
    exit 1
fi

echo "OK: the extension survived an in-process PHP thread reboot and still runs commands"
