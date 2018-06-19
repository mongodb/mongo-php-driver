<?php

use MongoDB\Driver\Command;
use MongoDB\Driver\Manager;
use MongoDB\Driver\ReadPreference;
use MongoDB\Driver\Server;
use MongoDB\Driver\Exception\ConnectionException;
use MongoDB\Driver\Exception\RuntimeException;

/**
 * Appends an option to a URI string and returns a new URI.
 *
 * @param string $uri
 * @param string $option
 * @param string $value
 * @return string
 */
function append_uri_option($uri, $option)
{
    // Append to existing query string
    if (strpos($uri, '?') !== false) {
        return $uri . '&' . $option;
    }

    // Terminate host list and append new query string
    if (parse_url($uri, PHP_URL_PATH) === null) {
        return $uri . '/?' . $option;
    }

    // Append query string after terminated host list and possible auth database
    return $uri . '?' . $option;
}

/**
 * Drops a collection on the primary server.
 *
 * @param string $uri            Connection string
 * @param string $databaseName   Database name
 * @param string $collectionName Collection name
 * @throws RuntimeException
 */
function drop_collection($uri, $databaseName, $collectionName)
{
    $server = get_primary_server($uri);
    $command = new Command(['drop' => $collectionName]);

    try {
        /* We need to use WriteConcern::MAJORITY here due to the issue
         * explained in SERVER-35613: "drop" uses a two phase commit, and due
         * to that, it is possible that a lock can't be acquired for a
         * transaction that gets quickly started as the "drop" reaper hasn't
         * completed yet. */
        $server->executeCommand(
            $databaseName,
            $command,
            [ 'writeConcern' => new \MongoDB\Driver\WriteConcern( \MongoDB\Driver\WriteConcern::MAJORITY ) ]
        );
    } catch (RuntimeException $e) {
        if ($e->getMessage() !== 'ns not found') {
            throw $e;
        }
    }
}

/**
 * Returns the value of a module row from phpinfo(), or null if it's not found.
 *
 * @param string $row
 * @return string|null
 */
function get_module_info($row)
{
    ob_start();
    phpinfo(INFO_MODULES);
    $info = ob_get_clean();

    $pattern = sprintf('/^%s([\w ]+)$/m', preg_quote($row . ' => '));

    if (preg_match($pattern, $info, $matches) !== 1) {
        return null;
    }

    return $matches[1];
}

/**
 * Returns the primary server.
 *
 * @param string $uri Connection string
 * @return Server
 * @throws ConnectionException
 */
function get_primary_server($uri)
{
    return (new Manager($uri))->selectServer(new ReadPreference('primary'));
}

/**
 * Returns a parameter of the primary server.
 *
 * @param string $uri Connection string
 * @return mixed
 * @throws RuntimeException
 */
function get_server_parameter($uri, $parameter)
{
    $server = get_primary_server($uri);
    $command = new Command(['getParameter' => 1, $parameter => 1]);
    $cursor = $server->executeCommand('admin', $command);

    return current($cursor->toArray())->$parameter;
}

/**
 * Returns the storage engine of the primary server.
 *
 * @param string $uri Connection string
 * @return string
 * @throws RuntimeException
 */
function get_server_storage_engine($uri)
{
    $server = get_primary_server($uri);
    $command = new Command(['serverStatus' => 1]);
    $cursor = $server->executeCommand('admin', $command);

    return current($cursor->toArray())->storageEngine->name;
}

/**
 * Returns the version of the primary server.
 *
 * @param string $uri Connection string
 * @return string
 * @throws RuntimeException
 */
function get_server_version($uri)
{
    $server = get_primary_server($uri);
    $command = new Command(['buildInfo' => 1]);
    $cursor = $server->executeCommand('admin', $command);

    return current($cursor->toArray())->version;
}

/**
 * Returns the value of a URI option, or null if it's not found.
 *
 * @param string $uri
 * @return string|null
 */
function get_uri_option($uri, $option)
{
    $pattern = sprintf('/[?&]%s=([^&]+)/i', preg_quote($option));

    if (preg_match($pattern, $uri, $matches) !== 1) {
        return null;
    }

    return $matches[1];
}

/**
 * Checks that the topology is a sharded cluster.
 *
 * @param string $uri
 * @return boolean
 */
function is_mongos($uri)
{
    return get_primary_server($uri)->getType() === Server::TYPE_MONGOS;
}

/**
 * Checks that the topology is a replica set.
 *
 * @param string $uri
 * @return boolean
 */
function is_replica_set($uri)
{
    return get_primary_server($uri)->getType() === Server::TYPE_RS_PRIMARY;
}

/**
 * Checks if the connection string uses authentication.
 *
 * @param string $uri
 * @return boolean
 */
function is_auth($uri)
{
    if (stripos($uri, 'authmechanism=') !== false) {
        return true;
    }

    if (strpos($uri, ':') !== false && strpos($uri, '@') !== false) {
        return true;
    }

    return false;
}

/**
 * Checks if the connection string uses SSL.
 *
 * @param string $uri
 * @return boolean
 */
function is_ssl($uri)
{
    return stripos($uri, 'ssl=true') !== false;
}

/**
 * Checks that the topology is a standalone.
 *
 * @param string $uri
 * @return boolean
 */
function is_standalone($uri)
{
    return get_primary_server($uri)->getType() === Server::TYPE_STANDALONE;
}

/**
 * Converts the server type constant to a string.
 *
 * @see http://php.net/manual/en/class.mongodb-driver-server.php
 * @param integer $type
 * @return string
 */
function server_type_as_string($type)
{
    switch ($type) {
        case Server::TYPE_STANDALONE:
            return 'Standalone';
        case Server::TYPE_MONGOS:
            return 'Mongos';
        case Server::TYPE_POSSIBLE_PRIMARY:
            return 'PossiblePrimary';
        case Server::TYPE_RS_PRIMARY:
            return 'RSPrimary';
        case Server::TYPE_RS_SECONDARY:
            return 'RSSecondary';
        case Server::TYPE_RS_ARBITER:
            return 'RSArbiter';
        case Server::TYPE_RS_OTHER:
            return 'RSOther';
        case Server::TYPE_RS_GHOST:
            return 'RSGhost';
        default:
            return 'Unknown';
    }
}

/**
 * Converts an errno number to a string.
 *
 * @see http://php.net/manual/en/errorfunc.constants.php
 * @param integer $errno
 * @param string
 */
function errno_as_string($errno)
{
    $errors = [
        'E_ERROR',
        'E_WARNING',
        'E_PARSE',
        'E_NOTICE',
        'E_CORE_ERROR',
        'E_CORE_WARNING',
        'E_COMPILE_ERROR',
        'E_COMPILE_WARNING',
        'E_USER_ERROR',
        'E_USER_WARNING',
        'E_USER_NOTICE',
        'E_STRICT',
        'E_RECOVERABLE_ERROR',
        'E_DEPRECATED',
        'E_USER_DEPRECATED',
        'E_ALL',
    ];

    foreach ($errors as $error) {
        if ($errno === constant($error)) {
            return $error;
        }
    }

    return 'Unknown';
}

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
 * Canonicalizes a JSON string.
 *
 * @param string $json
 * @return string
 */
function json_canonicalize($json)
{
    $json = json_encode(json_decode($json));

    /* Versions of PHP before 7.1 replace empty JSON keys with "_empty_" when
     * decoding to a stdClass (see: https://bugs.php.net/bug.php?id=46600). Work
     * around this by replacing "_empty_" keys before returning.
     */
    return str_replace('"_empty_":', '"":', $json);
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

function NEEDS($configuration) {
    if (!constant($configuration)) {
        exit("skip -- need '$configuration' defined");
    }
}
function PREDICTABLE() {
    global $servers;

    foreach($servers as $k => $v) {
        if (!defined($k) || !constant($k)) {
            exit("skip - needs predictable environment (e.g. vagrant)\n");
        }
    }
}
function SLOW() {
    if (getenv("SKIP_SLOW_TESTS")) {
        exit("skip SKIP_SLOW_TESTS");
    }
}
function LOAD($uri, $dbname = DATABASE_NAME, $collname = COLLECTION_NAME, $filename = null) {
    if (!$filename) {
        $filename = "compress.zlib://" . __DIR__ . "/" . "PHONGO-FIXTURES.json.gz";
    }

    $manager = new MongoDB\Driver\Manager($uri);
    $bulk = new MongoDB\Driver\BulkWrite(['ordered' => false]);

    $server = $manager->selectServer(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY));

    $data = file_get_contents($filename);
    $array = json_decode($data);

    foreach($array as $document) {
        $bulk->insert($document);
    }

    $retval = $server->executeBulkWrite("$dbname.$collname", $bulk);

    if ($retval->getInsertedCount() !== count($array)) {
        exit(sprintf('skip Fixtures were not loaded (expected: %d, actual: %d)', $total, $retval->getInsertedCount()));
    }
}

function START($id, array $options = array()) {
    /* starting/stopping servers only works using the Vagrant setup */
    PREDICTABLE();

    $options += array("name" => "mongod", "id" => $id);
    $opts = array(
        "http" => array(
            "timeout" => 60,
            "method"  => "PUT",
            "header"  => "Accept: application/json\r\n" .
                         "Content-type: application/x-www-form-urlencoded",
            "content" => json_encode($options),
            "ignore_errors" => true,
        ),
    );
    $ctx = stream_context_create($opts);
    $json = file_get_contents(getMOUri() . "/servers/$id", false, $ctx);
    $result = json_decode($json, true);

    /* Failed -- or was already started */
    if (!isset($result["mongodb_uri"])) {
        DELETE($id);
        define($id, false);
    } else {
        define($id, $result["mongodb_uri"]);
        $FILENAME = sys_get_temp_dir() . "/PHONGO-SERVERS.json";

        $json = file_get_contents($FILENAME);
        $config = json_decode($json, true);
        $config[$id] = constant($id);
        file_put_contents($FILENAME, json_encode($config, JSON_PRETTY_PRINT));
    }
}
function DELETE($id) {
    $opts = array(
        "http" => array(
            "timeout" => 60,
            "method"  => "DELETE",
            "header"  => "Accept: application/json\r\n",
            "ignore_errors" => true,
        ),
    );
    $ctx = stream_context_create($opts);
    $json = file_get_contents(getMOUri() . "/servers/$id", false, $ctx);
        $FILENAME = sys_get_temp_dir() . "/PHONGO-SERVERS.json";

        $json = file_get_contents($FILENAME);
        $config = json_decode($json, true);
        unset($config[$id]);
        file_put_contents($FILENAME, json_encode($config, JSON_PRETTY_PRINT));
}
function severityToString($type) {
    switch($type) {
    case E_WARNING:
        return "E_WARNING";
    default:
        return "Some other #_$type";
    }
}
function raises($function, $type, $infunction = null) {
    $errhandler = function($severity, $message, $file, $line, $errcontext) {
        throw new ErrorException($message, 0, $severity, $file, $line);
    };

    set_error_handler($errhandler, $type);
    try {
        $function();
    } catch(Exception $e) {
        if ($e instanceof ErrorException && $e->getSeverity() & $type) {
            if ($infunction) {
                $trace = $e->getTrace();
                $function = $trace[0]["function"];
                if (strcasecmp($function, $infunction) == 0) {
                    printf("OK: Got %s thrown from %s\n", $exceptionname, $infunction);
                } else {
                    printf("ALMOST: Got %s - but was thrown in %s, not %s\n", $exceptionname, $function, $infunction);
                }
                restore_error_handler();
                return $e->getMessage();
            }
            printf("OK: Got %s\n", severityToString($type));
        } else {
            printf("ALMOST: Got %s - expected %s\n", get_class($e), $exceptionname);
        }
        restore_error_handler();
        return $e->getMessage();
    }

    echo "FAILED: Expected $exceptionname thrown!\n";
    restore_error_handler();
}
function throws($function, $exceptionname, $infunction = null) {
    try {
        $function();
    } catch(Exception $e) {
        $message = str_replace(array("\n", "\r"), ' ', $e->getMessage());
        if ($e instanceof $exceptionname) {
            if ($infunction) {
                $trace = $e->getTrace();
                $function = $trace[0]["function"];
                if (strcasecmp($function, $infunction) == 0) {
                    printf("OK: Got %s thrown from %s\n", $exceptionname, $infunction);
                } else {
                    printf("ALMOST: Got %s - but was thrown in %s, not %s (%s)\n", $exceptionname, $function, $infunction, $message);
                }
                return $e->getMessage();
            }
            printf("OK: Got %s\n", $exceptionname);
        } else {
            printf("ALMOST: Got %s (%s) - expected %s\n", get_class($e), $message, $exceptionname);
        }
        return $e->getMessage();
    }
    echo "FAILED: Expected $exceptionname thrown, but no exception thrown!\n";
}
function printServer(MongoDB\Driver\Server $server)
{
    printf("server: %s:%d\n", $server->getHost(), $server->getPort());
}

function printWriteResult(MongoDB\Driver\WriteResult $result, $details = true)
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
    printWriteConcernError($writeConcernError ? $writeConcernError : null, $details);

    foreach ($result->getWriteErrors() as $writeError) {
        printWriteError($writeError);
    }
}

function printWriteConcernError(MongoDB\Driver\WriteConcernError $error = null, $details)
{
    if ($error) {
        /* This stuff is generated by the server, no need for us to test it */
        if (!$details) {
            printf("writeConcernError: %s (%d)\n", $error->getMessage(), $error->getCode());
            return;
        }
        var_dump($error);
        printf("writeConcernError.message: %s\n", $error->getMessage());
        printf("writeConcernError.code: %d\n", $error->getCode());
        printf("writeConcernError.info: ");
        var_dump($error->getInfo());
    }
}

function printWriteError(MongoDB\Driver\WriteError $error)
{
    var_dump($error);
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

function configureFailPoint(MongoDB\Driver\Manager $manager, $failPoint, $mode, $data = array()) {

    $doc = array(
        "configureFailPoint" => $failPoint,
        "mode"               => $mode,
    );
    if ($data) {
        $doc["data"] = $data;
    }

    $cmd = new MongoDB\Driver\Command($doc);
    $result = $manager->executeCommand("admin", $cmd);
    $arr = current($result->toArray());
    if (empty($arr->ok)) {
        var_dump($result);
        throw new RuntimeException("Failpoint failed");
    }
    return true;
}

function failMaxTimeMS(MongoDB\Driver\Manager $manager) {
    return configureFailPoint($manager, "maxTimeAlwaysTimeOut", array("times" => 1));
}

function getMOUri() {
    if (!($HOST = getenv("MONGODB_ORCHESTRATION_HOST"))) {
        $HOST = "192.168.112.10";
    }

    if (!($PORT = getenv("MONGODB_ORCHESTRATION_PORT"))) {
        $PORT = "8889";
    }
    $MO = "http://$HOST:$PORT/v1";

    return $MO;
}

function getMOPresetBase() {
    if (!($BASE = getenv("mongodb_orchestration_base"))) {
        $BASE = "/phongo/";
    }

    return $BASE;
}

function toPHP($var, $typemap = array()) {
    return MongoDB\BSON\toPHP($var, $typemap);
}
function fromPHP($var) {
    return MongoDB\BSON\fromPHP($var);
}
function toJSON($var) {
    return MongoDB\BSON\toJSON($var);
}
function toCanonicalExtendedJSON($var) {
    return MongoDB\BSON\toCanonicalExtendedJSON($var);
}
function toRelaxedExtendedJSON($var) {
    return MongoDB\BSON\toRelaxedExtendedJSON($var);
}
function fromJSON($var) {
    return MongoDB\BSON\fromJSON($var);
}

/* Note: this fail point may terminate the mongod process, so you may want to
 * use this in conjunction with a throwaway server. */
function failGetMore(MongoDB\Driver\Manager $manager) {
    return configureFailPoint($manager, "failReceivedGetmore", "alwaysOn");
}
