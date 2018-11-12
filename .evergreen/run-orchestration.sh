#!/bin/sh
set -o xtrace   # Write all commands first to stderr
set -o errexit  # Exit the script with error if any of the commands fail


AUTH=${AUTH:-noauth}
SSL=${SSL:-nossl}
TOPOLOGY=${TOPOLOGY:-server}
STORAGE_ENGINE=${STORAGE_ENGINE}
MONGODB_VERSION=${MONGODB_VERSION:-latest}

DL_START=$(date +%s)
DIR=$(dirname $0)
# Functions to fetch MongoDB binaries
. ${DRIVERS_TOOLS}/.evergreen/download-mongodb.sh

get_distro
if [ -z "$MONGODB_DOWNLOAD_URL" ]; then
    get_mongodb_download_url_for "$DISTRO" "$MONGODB_VERSION"
else
  # Even though we have the MONGODB_DOWNLOAD_URL, we still call this to get the proper EXTRACT variable
  get_mongodb_download_url_for "$DISTRO"
fi
download_and_extract "$MONGODB_DOWNLOAD_URL" "$EXTRACT"

DL_END=$(date +%s)
MO_START=$(date +%s)

END_POINT=${TOPOLOGY}
if [ "standalone" = "${TOPOLOGY}" ]; then
    END_POINT="servers"
fi

export ORCHESTRATION_FILE="$PROJECT_DIRECTORY/scripts/presets/travis/${TOPOLOGY}/${CONFIG}.json"
export ORCHESTRATION_URL="http://localhost:8889/v1/${END_POINT}"

# Start mongo-orchestration
sh ${PROJECT_DIRECTORY}/.evergreen/start-orchestration.sh "$MONGO_ORCHESTRATION_HOME"

pwd
cat "$ORCHESTRATION_FILE"
curl --silent --show-error --data @"$ORCHESTRATION_FILE" "$ORCHESTRATION_URL" --max-time 600 --fail -o tmp.json
cat tmp.json
URI=$(python -c 'import sys, json; j=json.load(open("tmp.json")); print(j["mongodb_auth_uri" if "mongodb_auth_uri" in j else "mongodb_uri"])' | tr -d '\r')
echo 'MONGODB_URI: "'$URI'"' > mo-expansion.yml
echo "Cluster URI: $URI"

MO_END=$(date +%s)
MO_ELAPSED=$(expr $MO_END - $MO_START)
DL_ELAPSED=$(expr $DL_END - $DL_START)
cat <<EOT >> $DRIVERS_TOOLS/results.json
{"results": [
  {
    "status": "PASS",
    "test_file": "Orchestration",
    "start": $MO_START,
    "end": $MO_END,
    "elapsed": $MO_ELAPSED
  },
  {
    "status": "PASS",
    "test_file": "Download MongoDB",
    "start": $DL_START,
    "end": $DL_END,
    "elapsed": $DL_ELAPSED
  }
]}

EOT
