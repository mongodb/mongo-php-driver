<?php
require __DIR__ . "/" . "../tests/utils/orchestration.php";

if (!($host = getenv("MONGODB_ORCHESTRATION"))) {
    $host = "http://192.168.112.10:8889";
}

$orch = new Mongo\Orchestration($host, getenv("MONGODB_ORCHESTRATION_PRESETS_ROOT"));
if (!$orch->ping()) {
    echo "Failed connecting to MO\n";
    exit(3);
}

foreach($orch->getAll() as $uri => $server) {
    printf("%s:%s%s\n", $server["id"], str_repeat(" ", 40-strlen($server["id"])), $server["mongodb_uri"]);
}


