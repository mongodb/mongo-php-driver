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

