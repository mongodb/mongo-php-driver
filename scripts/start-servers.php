<?php
require __DIR__ . "/" . "../tests/utils/tools.php";
$SERVERS = array();
$FILENAME = sys_get_temp_dir() . "/PHONGO-SERVERS.json";

ini_set("default_socket_timeout", 60000);

function lap() {
    static $then = 0;
    static $now;

    $now = microtime(true);
    $ret = $now - $then;
    $then = $now;
    return $ret;
}


$PRESETS = [
    "standalone" => [
        "scripts/presets/standalone.json",
        "scripts/presets/standalone-24.json",
        "scripts/presets/standalone-26.json",
        "scripts/presets/standalone-30.json",
        "scripts/presets/standalone-ssl.json",
        "scripts/presets/standalone-auth.json",
        "scripts/presets/standalone-x509.json",
        "scripts/presets/standalone-plain.json",
    ],
    "replicasets" => [
        "scripts/presets/replicaset.json",
        "scripts/presets/replicaset-30.json",
    ],
];

function make_ctx($preset, $method = "POST") {
    $opts = [
        "http" => [
            "timeout" => 60,
            "method"  => $method,
            "header"  => "Accept: application/json\r\n" .
                         "Content-type: application/x-www-form-urlencoded",
            "content" => json_encode(array("preset" => $preset)),
            "ignore_errors" => true,
        ],
    ];
    $ctx = stream_context_create($opts);
    return $ctx;
}

function failed($result) {
    echo "\n\n";
    echo join("\n", $result);
    printf("Last operation took: %.2f secs\n", lap());
    exit();
}

function mo_http_request($uri, $context) {
    global $http_response_header;

    $result = file_get_contents($uri, false, $context);

    if ($result === false) {
        failed($http_response_header);
    }

    return $result;
}

printf("Cleaning out previous processes, if any ");
lap();
/* Remove all pre-existing ReplicaSets */
$replicasets = mo_http_request(getMOUri() . "/replica_sets", make_ctx(getMOPresetBase(), "GET"));
$replicasets = json_decode($replicasets, true);
foreach($replicasets["replica_sets"] as $replicaset) {
    $uri = getMOUri() . "/replica_sets/" . $replicaset["id"];
    mo_http_request($uri, make_ctx(getMOPresetBase(), "DELETE"));
    echo ".";
}
echo " ";
/* Remove all pre-existing servers */
$servers = mo_http_request(getMOUri() . "/servers", make_ctx(getMOPresetBase(), "GET"));
$servers = json_decode($servers, true);
foreach($servers["servers"] as $server) {
    $uri = getMOUri() . "/servers/" . $server["id"];
    mo_http_request($uri, make_ctx(getMOPresetBase(), "DELETE"));
    echo ".";
}
printf("\t(took: %.2f secs)\n", lap());

foreach($PRESETS["standalone"] as $preset) {
    lap();
    $json = json_decode(file_get_contents($preset), true);
    printf("Starting %-20s ...  ", $json["id"]);

    $result = mo_http_request(getMOUri() . "/servers", make_ctx(getMOPresetBase() . $preset));
    $decode = json_decode($result, true);
    if (!isset($decode["id"])) {
        failed($decode);
    }

    $SERVERS[$decode["id"]] = isset($decode["mongodb_auth_uri"]) ? $decode["mongodb_auth_uri"] : $decode["mongodb_uri"];
    printf("'%s'\t(took: %.2f secs)\n", $SERVERS[$decode["id"]], lap());
}
echo "---\n";

foreach($PRESETS["replicasets"] as $preset) {
    lap();
    $json = json_decode(file_get_contents($preset), true);
    printf("Starting %-20s ...  ", $json["id"]);

    $result = mo_http_request(getMOUri() . "/replica_sets", make_ctx(getMOPresetBase() . $preset));
    $decode = json_decode($result, true);
    if (!isset($decode["id"])) {
        failed($decode);
    }
    $SERVERS[$decode["id"]] = isset($decode["mongodb_auth_uri"]) ? $decode["mongodb_auth_uri"] : $decode["mongodb_uri"];
    printf("'%s'\t(took: %.2f secs)\n", $SERVERS[$decode["id"]], lap());
}

file_put_contents($FILENAME, json_encode($SERVERS, JSON_PRETTY_PRINT));

/*
wget --body-data='' --method='GET' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/servers
wget --body-data='' --method='DELETE' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/servers/STANDALONE-AUTH
wget --body-data='' --method='DELETE' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/servers/STANDALONE
wget --body-data='' --method='DELETE' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/servers/STANDALONE-24
wget --body-data='' --method='DELETE' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/servers/STANDALONE-26
wget --body-data='' --method='DELETE' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/servers/RS-two
wget --body-data='' --method='DELETE' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/servers/RS-arbiter
wget --body-data='' --method='DELETE' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/servers/STANDALONE-PLAIN
wget --body-data='' --method='DELETE' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/servers/STANDALONE-X509
wget --body-data='' --method='DELETE' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/servers/RS-one
wget --body-data='' --method='DELETE' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/servers/STANDALONE-SSL

wget --body-data='' --method='GET' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/replica_sets
wget --body-data='' --method='DELETE' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/replica_sets/REPLICASET

wget --body-data='' --method='GET' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/
wget --body-data='' --method='GET' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/servers
wget --body-data='' --method='GET' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/replica_sets

wget --body-data='{"preset":"\/phongo\/\/scripts\/presets\/standalone.json"}' --method='POST' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/servers
wget --body-data='{"preset":"\/phongo\/\/scripts\/presets\/standalone-24.json"}' --method='POST' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/servers
wget --body-data='{"preset":"\/phongo\/\/scripts\/presets\/standalone-26.json"}' --method='POST' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/servers
wget --body-data='{"preset":"\/phongo\/\/scripts\/presets\/standalone-ssl.json"}' --method='POST' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/servers
wget --body-data='{"preset":"\/phongo\/\/scripts\/presets\/standalone-auth.json"}' --method='POST' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/servers
wget --body-data='{"preset":"\/phongo\/\/scripts\/presets\/standalone-x509.json"}' --method='POST' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/servers
wget --body-data='{"preset":"\/phongo\/\/scripts\/presets\/standalone-plain.json"}' --method='POST' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/servers
wget --body-data='{"preset":"\/phongo\/\/scripts\/presets\/replicaset.json"}' --method='POST' --header='Accept: application/json' --header='Content-type: application/x-www-form-urlencoded'  http://192.168.112.10:8889/replica_sets
*/
