<?php

/* Stolen from the intertubes */
function hex_dump($data)
{
    static $from = '';
    static $to = '';

    static $width = 16; # number of bytes per line

    static $pad = '.'; # padding for non-visible characters

    if ($from==='')
    {
        for ($i=0; $i<=0xFF; $i++)
        {
            $from .= chr($i);
            $to .= ($i >= 0x20 && $i <= 0x7E) ? chr($i) : $pad;
        }
    }

    $hex = str_split(bin2hex($data), $width*2);
    $chars = str_split(strtr($data, $from, $to), $width);

    $offset = 0;
    foreach ($hex as $i => $line)
    {
        $length = $width * 3;
        $dump = sprintf("%-{$length}s", implode(' ', str_split($line,2)));
        echo sprintf('%6X',$offset).' : '.$dump . ' [' . $chars[$i] . ']'."\n";
        $offset += $width;
    }
}

function makeCollectionNameFromFilename($filename) {
    $rp = realpath($filename);

    $search = "tests". DIRECTORY_SEPARATOR;
    // relative path from tests/
    $base = substr($rp, strpos($rp, $search)+strlen($search));
    $dir = dirname($base) . DIRECTORY_SEPARATOR;
    // Its the root of tests/
    if ($dir[0] == ".") {
        $dir = "";
    }

    $basename = basename($filename);
    /* SKIPIFs are named [basename].skip.php */
    $base = strstr($basename, ".", true);

    // directory/testname
    $retval = $dir . $base;

    // directory-testname
    $retval = str_replace(DIRECTORY_SEPARATOR, "-", $retval);

    return $retval;
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


