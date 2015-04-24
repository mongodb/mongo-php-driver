--TEST--
Connect to MongoDB with using PLAIN auth mechanism #002
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; NEEDS("STANDALONE_PLAIN"); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$username = "root";
$password = "toor";
$database = "admin";

$parsed = parse_url(STANDALONE_PLAIN);
$dsn = sprintf("mongodb://%s:%s@%s:%d/%s", $username, $password, $parsed["host"], $parsed["port"], $database);
$adminmanager = new MongoDB\Driver\Manager($dsn);

$cmd = array(
    "createUser" => "bugs",
    "roles" => array(array("role" => "readWrite", "db" => DATABASE_NAME)),
);
$command = new MongoDB\Driver\Command($cmd);
try {
    $result = $adminmanager->executeCommand('$external', $command);
    echo "User Created\n";
} catch(Exception $e) {
    echo $e->getMessage(), "\n";
}



$username = "bugs";
$password = "wrong-password";
$database = '$external';

$dsn = sprintf("mongodb://%s:%s@%s:%d/%s?authMechanism=PLAIN", $username, $password, $parsed["host"], $parsed["port"], $database);
$manager = new MongoDB\Driver\Manager($dsn);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(array("very" => "important"));
throws(function() use($manager, $bulk) {
    $manager->executeBulkWrite(NS, $bulk);
}, "MongoDB\Driver\Exception\AuthenticationException");

$cmd = array(
    "dropUser" => "bugs",
);
$command = new MongoDB\Driver\Command($cmd);
try {
    $result = $adminmanager->executeCommand('$external', $command);
    echo "User deleted\n";
} catch(Exception $e) {
    echo $e->getMessage(), "\n";
}
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
User Created
OK: Got MongoDB\Driver\Exception\AuthenticationException
User deleted
===DONE===
