<?php
require __DIR__ . "/" . "orchestration.php";
require __DIR__ . "/" . "tools.php";


$host = "http://192.168.112.10:8889";
if ($_ENV && isset($_ENV["ORCHESTRATION"])) {
    $host = $_ENV["ORCHESTRATION"];
}


$orch = new Mongo\Orchestration($host);
if (!$orch->ping()) {
    exit("skip Mongo Orchestration not running on {$host} - run 'make test-bootstrap'");
}

$consts = array(
    "MONGODB_URI"         => $orch->getURI("standalone.json"),
    "MONGODB_CLEANUP_URI" => $orch->getURI("standalone.json"),
    "DATABASE_NAME"       => "phongo",
    "COLLECTION_NAME"     => makeCollectionNameFromFilename($_SERVER["SCRIPT_FILENAME"]),
    "DEBUG_DIR"           => sys_get_temp_dir() . "/PHONGO-TESTS/",
);
def($consts);

// These use values from constants defined above
$consts = array(
    "NS"                  => DATABASE_NAME . "." . COLLECTION_NAME,
    "DEBUG_FILENAME"      => DEBUG_DIR . DATABASE_NAME . "." . COLLECTION_NAME,
);
def($consts);



if (!is_dir(DEBUG_DIR)) {
    mkdir(DEBUG_DIR, 0777, true);
}

ini_set("phongo.debug_log", DEBUG_FILENAME);
file_put_contents(DEBUG_FILENAME, sprintf("===> %s <=== %s\n", date(DATE_ISO8601), $_SERVER["SCRIPT_FILENAME"]), FILE_APPEND);


