<?php
require __DIR__ . "/" . "tools.php";

if (($FILENAME = getenv("PHONGO_SERVERS")) === false) {
	$FILENAME = sys_get_temp_dir() . "/PHONGO-SERVERS.json";
}

$json = file_get_contents($FILENAME);
$config = json_decode($json, true);
if (!$config) {
    exit("skip Couldn't json_decode(file_get_contents($FILENAME));");
}

$servers = array(
    "STANDALONE"      => "",
    "STANDALONE_24"   => "",
    "STANDALONE_26"   => "",
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
);
def($consts);

// These use values from constants defined above
$consts = array(
    "NS"             => DATABASE_NAME . "." . COLLECTION_NAME,
);
def($consts);

/* If its not defined, set it to what the MongoDB exts define it, since this is mostly used for pecl/bson testing */
if (!defined("BSON_NAMESPACE")) {
    define("BSON_NAMESPACE", "MongoDB\\BSON");
}

