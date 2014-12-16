<?php
require __DIR__ . "/" . "tools.php";


}


$consts = array(
    "MONGODB_URI"         => "mongodb://localhost:27017",
    "MONGODB_CLEANUP_URI" => "mongodb://localhost:27017",
    "DATABASE_NAME"       => "phongo_test",
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


