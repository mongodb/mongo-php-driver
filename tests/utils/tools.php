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
        echo "skip (cleanup)" . $e->getCode(), ": ", $e->getMessage();
        exit(1);
    }
}

function throws(callable $function, $exceptionname, $infunction = null) {
    try {
        $function();
    } catch(Exception $e) {
        if ($e instanceof $exceptionname) {
            if ($infunction) {
                $function = $e->getTrace()[0]["function"];
                if (strcasecmp($function, $infunction) == 0) {
                    printf("OK: Got %s thrown from %s\n", $exceptionname, $infunction);
                } else {
                    printf("ALMOST: Got %s - but was thrown in %s, not %s\n", $exceptionname, $function, $infunction);
                }
                return $e->getMessage();
            }
            printf("OK: Got %s\n", $exceptionname);
        } else {
            printf("ALMOST: Got %s - expected %s\n", get_class($e), $exceptionname);
        }
        return $e->getMessage();
    }
    echo "FAILED: Expected $exceptionname thrown!\n";
}
function printServer(MongoDB\Server $server)
{
    printf("server: %s:%d\n", $server->getHost(), $server->getPort());
}

function printWriteResult(MongoDB\WriteResult $result)
{
    printServer($result->getServer());

    printf("insertedCount: %d\n", $result->getInsertedCount());
    printf("matchedCount: %d\n", $result->getMatchedCount());
    printf("modifiedCount: %d\n", $result->getModifiedCount());
    printf("upsertedCount: %d\n", $result->getUpsertedCount());
    printf("deletedCount: %d\n", $result->getDeletedCount());

    foreach ($result->getUpsertedIds() as $index => $id) {
        printf("upsertedId[%d]: ", $index);
        var_dump($id);
    }

    $writeConcernError = $result->getWriteConcernError();
    printWriteConcernError($writeConcernError);

    foreach ($result->getWriteErrors() as $writeError) {
        printWriteError($writeError);
    }
}

function printWriteConcernError(MongoDB\WriteConcernError $error = null)
{
    if ($error) {
        printf("writeConcernError.message: %s\n", $error->getMessage());
        printf("writeConcernError.code: %d\n", $error->getCode());
        printf("writeConcernError.info: ");
        var_dump($error->getInfo());
    }
}

function printWriteError(MongoDB\WriteError $error)
{
    printf("writeError[%d].message: %s\n", $error->getIndex(), $error->getMessage());
    printf("writeError[%d].code: %d\n", $error->getIndex(), $error->getCode());
}

function getInsertCount($retval) {
    return $retval->getInsertedCount();
}
function getModifiedCount($retval) {
    return $retval->getModifiedCount();
}
function getDeletedCount($retval) {
    return $retval->getDeletedCount();
}
function getUpsertedCount($retval) {
    return $retval->getUpsertedCount();
}
function getWriteErrors($retval) {
    return (array)$retval->getWriteErrors();
}

function def($arr) {
    foreach($arr as $const => $value) {
        define($const, getenv("PHONGO_TEST_$const") ?: $value); 
    }
}

