<?php
require __DIR__ . "/" . "tools.php";


$FILENAME = sys_get_temp_dir() . "/PHONGO-SERVERS.json";

$json = file_get_contents($FILENAME);
$config = json_decode($json, true);
if (!$config) {
    exit("skip Couldn't json_decode(file_get_contents($FILENAME));");
}

$servers = array(
    "STANDALONE"      => "",
    "STANDALONE_SSL"  => "",
    "STANDALONE_AUTH" => "",
    "STANDALONE_X509" => "",
    "STANDALONE_PLAIN" => "",
    "REPLICASET"       => "",
);
$servers = array_merge($servers, $config);
def($servers);

$consts = array(
    "DATABASE_NAME"                 => "phongo",
    "COLLECTION_NAME"               => makeCollectionNameFromFilename($_SERVER["SCRIPT_FILENAME"]),
    "DEBUG_DIR"                     => sys_get_temp_dir() . "/PHONGO-TESTS/",
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

/* If the INI option hasn't been changed, then lets set it to the debug log */
$ininame = "phongo.debug_log";
$origin = ini_get($ininame);
ini_restore($ininame);
if ($ininame == $origin) {
    ini_set("phongo.debug_log", DEBUG_FILENAME);
    file_put_contents(DEBUG_FILENAME, sprintf("===> %s <=== %s\n", date(DATE_ISO8601), $_SERVER["SCRIPT_FILENAME"]), FILE_APPEND);
}


