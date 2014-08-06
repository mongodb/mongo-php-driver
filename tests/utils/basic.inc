<?php

/**
 * Prints a traditional hex dump of byte values and printable characters.
 *
 * @see http://stackoverflow.com/a/4225813/162228
 * @param string $data   Binary data
 * @param integer $width Bytes displayed per line
 */
function hex_dump($data, $width = 16)
{
    static $pad = '.'; // Placeholder for non-printable characters
    static $from = '';
    static $to = '';

    if ($from === '') {
        for ($i = 0; $i <= 0xFF; $i++) {
            $from .= chr($i);
            $to .= ($i >= 0x20 && $i <= 0x7E) ? chr($i) : $pad;
        }
    }

    $hex = str_split(bin2hex($data), $width * 2);
    $chars = str_split(strtr($data, $from, $to), $width);

    $offset = 0;
    $length = $width * 3;

    foreach ($hex as $i => $line) {
        printf("%6X : %-{$length}s [%s]\n", $offset, implode(' ', str_split($line, 2)), $chars[$i]);
        $offset += $width;
    }
}

/**
 * Return a collection name to use for the test file.
 *
 * The filename will be stripped of the base path to the test suite (prefix) as
 * well as the PHP file extension (suffix). Special characters (including hyphen
 * for shell compatibility) will be replaced with underscores.
 *
 * @param string $filename
 * @return string
 */
function makeCollectionNameFromFilename($filename)
{
    $filename = realpath($filename);
    $prefix = realpath(dirname(__FILE__) . '/..') . DIRECTORY_SEPARATOR;

    $replacements = array(
        // Strip test path prefix
        sprintf('/^%s/', preg_quote($prefix, '/')) => '',
        // Strip file extension suffix
        '/\.php$/' => '',
        // SKIPIFs add ".skip" between base name and extension
        '/\.skip$/' => '',
        // Replace special characters with underscores
        sprintf('/[%s]/', preg_quote('-$/\\', '/')) => '_',
    );

    return preg_replace(array_keys($replacements), array_values($replacements), $filename);
}

function LOAD_FIXTURES() {
    try {
        $mc = new MongoDB\Manager(MONGODB_CLEANUP_URI);
        $cmd = new MongoDB\Command(array("drop" => COLLECTION_NAME));
        $rp = new MongoDB\ReadPreference(MongoDB\ReadPreference::RP_PRIMARY);
        $retval = $mc->executeInsert(NS, array("my" => "demo", array("document" => "with", "data")));
    } catch(Exception $e) {
        echo "skip " . $e->getCode(), ": ", $e->getMessage();
    }
}
function CLEANUP() {
    try {
        $mc = new MongoDB\Manager(MONGODB_CLEANUP_URI);
        $cmd = new MongoDB\Command(array("drop" => COLLECTION_NAME));
        $rp = new MongoDB\ReadPreference(MongoDB\ReadPreference::RP_PRIMARY);
        try {
            $mc->executeCommand(DATABASE_NAME, $cmd, $rp);
        } catch(Exception $e) {
            do {
                /* ns not found */
                if ($e->getCode() == 59) {
                    continue;
                }
                throw $e;
            } while (0);
        }
    } catch(Exception $e) {
        echo "skip " . $e->getCode(), ": ", $e->getMessage();
    }
}

function printServer(MongoDB\Server $server)
{
    printf("server: %s:%d\n", $server->getHost(), $server->getPort());
}

function printWriteResult(MongoDB\WriteResult $result)
{
    printServer($result->getServer());

    printf("numInserted: %d\n", $result->getNumInserted());
    printf("numMatched: %d\n", $result->getNumMatched());
    printf("numModified: %d\n", $result->getNumModified());
    printf("numUpserted: %d\n", $result->getNumUpserted());
    printf("numRemoved: %d\n", $result->getNumRemoved());

    foreach ($result->getUpsertedIds() as $index => $id) {
        printf("upsertedId[%d]: ", $index);
        var_dump($id);
    }

    foreach ($result->getWriteConcernErrors() as $writeConcernError) {
        printWriteConcernError($writeConcernError);
    }

    foreach ($result->getWriteErrors() as $writeError) {
        printWriteError($writeError);
    }
}

function printWriteConcernError(MongoDB\WriteConcernError $error)
{
    printf("writeConcernError.message: %s\n", $error->getMessage());
    printf("writeConcernError.code: %d\n", $error->getCode());
    printf("writeConcernError.info: ");
    var_dump($error->info());
}

function printWriteError(MongoDB\WriteError $error)
{
    printf("writeError[%d].message: %s\n", $error->getIndex(), $error->getMessage());
    printf("writeError[%d].code: %d\n", $error->getIndex(), $error->getCode());
}

function getInsertCount($retval) {
    return $retval->getNumInserted();
}
function getModifiedCount($retval) {
    return $retval->getNumModified();
}
function getRemovedCount($retval) {
    return $retval->getNumRemoved();
}
function getUpsertedCount($retval) {
    return $retval->getNumUpserted();
}
function getWriteErrors($retval) {
    return (array)$retval->getWriteErrors();
}

function def($arr) {
    foreach($arr as $const => $value) {
        define($const, getenv("PHONGO_TEST_$const") ?: $value); 
    }
}
$consts = array(
    "MONGODB_URI"         => "mongodb://localhost:27017",
    "MONGODB_CLEANUP_URI" => "mongodb://localhost:27017",
    "DATABASE_NAME"       => "TESTS",
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


