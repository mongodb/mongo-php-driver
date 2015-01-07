<?php
require __DIR__ . "/" . "../tests/utils/orchestration.php";

function lap() {
    static $then = 0;
    static $now;

    $now = microtime(true);
    $ret = $now - $then;
    $then = $now;
    return $ret;
}

if (!($host = getenv("MONGODB_ORCHESTRATION"))) {
    $host = "http://192.168.112.10:8889";
}

$orch = new Mongo\Orchestration($host);
if (!$orch->ping()) {
    echo file_get_contents("server.log");
    echo "Failed starting MO\n";
    exit(3);
}

lap();
$orch->stopAll();

$res = $orch->start("standalone.json");
printf("Standalone running on:\t\t\t(took: %.2f secs)\t%s\n", lap(), $res);

if (getenv("TRAVIS")) {
    echo "Skipping special nodes on travis\n";
    exit;
}
$res = $orch->start("standalone-ssl.json");
printf("Standalone SSL running on:\t\t(took: %.2f secs)\t%s\n", lap(), $res);

$res = $orch->start("standalone-auth.json");
printf("Standalone Auth running on:\t\t(took: %.2f secs)\t%s\n", lap(), $res);

$res = $orch->start("standalone-x509.json");
printf("Standalone X509 Auth running on:\t(took: %.2f secs)\t%s\n", lap(), $res);

$res = $orch->start("standalone-plain.json");
printf("Standalone PLAIN Auth running on:\t(took: %.2f secs)\t%s\n", lap(), $res);

$res = $orch->startRS("replicaset.json");
printf("ReplicaSet running on:\t\t\t(took: %.2f secs)\t%s\n", lap(), $res);

