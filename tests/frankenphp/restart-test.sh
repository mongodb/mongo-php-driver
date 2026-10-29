#!/usr/bin/env bash
#
# Functional test for PHPC-2797: FrankenPHP reboots all its PHP threads
# in-process when opcache_reset() is called. The engine then runs GSHUTDOWN and
# MINIT again in the same process, while libmongoc can only be initialized once
# per process. Before the fix, the next Manager creation aborted the process in
# mongoc_handshake_data_append().
#
# Requires Docker. Run it with:
#   bash tests/frankenphp/restart-test.sh

set -o errexit
set -o nounset
set -o pipefail

REPO_ROOT=$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)

CONTAINER_NAME=${CONTAINER_NAME:-mongodb-frankenphp-restart}
IMAGE_NAME=${IMAGE_NAME:-mongodb-frankenphp}
PORT=${PORT:-8080}
BASE_URL="http://127.0.0.1:${PORT}"

cleanup() {
    docker logs "${CONTAINER_NAME}" 2>&1 || true
    docker rm --force --volumes "${CONTAINER_NAME}" >/dev/null 2>&1 || true
}
trap cleanup EXIT

docker build --tag "${IMAGE_NAME}" --file "${REPO_ROOT}/tests/frankenphp/Dockerfile" "${REPO_ROOT}"

# ZEND_DONT_UNLOAD_MODULES keeps the extension mapped when the engine shuts
# down, like SAPIs that restart the engine in-process. Without it, the extension
# is unloaded and reloaded with fresh global state, which hides PHPC-2797.
docker run --detach \
    --name "${CONTAINER_NAME}" \
    --env ZEND_DONT_UNLOAD_MODULES=1 \
    --publish "${PORT}:80" \
    "${IMAGE_NAME}" >/dev/null

# Wait for the worker to serve requests. Creating a Manager with a nonexistent
# server is enough: the driver handshake is appended when the Manager is created.
echo "Waiting for FrankenPHP..."
for _ in $(seq 1 60); do
    if curl --silent --fail --max-time 2 --output /dev/null "${BASE_URL}/"; then
        break
    fi
    sleep 1
done

get() {
    curl --silent --fail --max-time 30 "${BASE_URL}$1"
}

instance_before=$(get /)
echo "Worker instance before the reboot: ${instance_before}"

# opcache_reset() makes FrankenPHP reboot all its PHP threads in-process.
if [ "$(get /reset)" != "reset" ]; then
    echo "FAILED: opcache_reset() did not run, the PHP threads were not rebooted"
    exit 1
fi

# The worker script runs again in the new engine lifecycle, so it reports a new
# instance identifier. Requests retry while the threads are being rebooted. If
# the extension aborted the process, they fail.
instance_after=""
for _ in $(seq 1 30); do
    instance_after=$(get / || true)
    if [ -n "${instance_after}" ] && [ "${instance_after}" != "${instance_before}" ]; then
        break
    fi
    sleep 1
done

if [ "${instance_after}" = "${instance_before}" ]; then
    echo "FAILED: the PHP threads were not rebooted, the worker instance did not change"
    exit 1
fi

echo "Worker instance after the reboot: ${instance_after}"

if [ "$(docker inspect --format '{{.State.Running}}' "${CONTAINER_NAME}")" != "true" ]; then
    echo "FAILED: the FrankenPHP process died after the in-process reboot"
    exit 1
fi

echo "OK: the extension survived an in-process PHP thread reboot"
