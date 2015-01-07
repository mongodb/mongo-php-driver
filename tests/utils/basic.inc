<?php
require __DIR__ . "/" . "orchestration.php";
require __DIR__ . "/" . "tools.php";


if (!($host = getenv("MONGODB_ORCHESTRATION"))) {
    $host = "http://192.168.112.10:8889";
}

$orch = new Mongo\Orchestration($host, getenv("MONGODB_ORCHESTRATION_PRESETS_ROOT"));
if ($orch->ping()) {
    $consts = array(
        "MONGODB_URI"                   => $orch->getURI("standalone.json"),
        "MONGODB_CLEANUP_URI"           => $orch->getURI("standalone.json"),
        "MONGODB_STANDALONE_URI"        => $orch->getURI("standalone.json"),
        "MONGODB_STANDALONE_AUTH_URI"   => $orch->getURI("standalone-auth.json"),
        "MONGODB_STANDALONE_SSL_URI"    => $orch->getURI("standalone-ssl.json"),
        "MONGODB_STANDALONE_PLAIN_URI"  => $orch->getURI("standalone-plain.json"),
        "MONGODB_STANDALONE_X509_URI"   => $orch->getURI("standalone-x509.json"),
        "MONGODB_REPLICASET_URI"        => $orch->getURI("replicaset.json"),
    );
} else {
    $consts = array(
        "MONGODB_URI"                   => getenv("MONGODB_TEST_URI"),
        "MONGODB_CLEANUP_URI"           => getenv("MONGODB_TEST_URI"),
        "MONGODB_STANDALONE_URI"        => getenv("MONGODB_STANDALONE_URI"),
        "MONGODB_STANDALONE_AUTH_URI"   => getenv("MONGODB_STANDALONE_AUTH_URI"),
        "MONGODB_STANDALONE_SSL_URI"    => getenv("MONGODB_STANDALONE_SSL_URI"),
        "MONGODB_STANDALONE_PLAIN_URI"  => getenv("MONGODB_STANDALONE_PLAIN_URI"),
        "MONGODB_STANDALONE_X509_URI"   => getenv("MONGODB_STANDALONE_X509_URI"),
        "MONGODB_REPLICASET_URI"        => getenv("MONGODB_REPLICASET_URI"),
    );

}
def($consts);



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

/* If we are running the .phpt directly, don't mess with the ini settings */
if (substr($_SERVER["PHP_SELF"], -5) != ".phpt") {
    ini_set("phongo.debug_log", DEBUG_FILENAME);
    file_put_contents(DEBUG_FILENAME, sprintf("===> %s <=== %s\n", date(DATE_ISO8601), $_SERVER["SCRIPT_FILENAME"]), FILE_APPEND);
}


