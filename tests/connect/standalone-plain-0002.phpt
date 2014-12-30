--TEST--
Connect to MongoDB with using PLAIN auth mechanism #002
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$username = "root";
$password = "toor";
$database = "admin";

$parsed = parse_url(MONGODB_STANDALONE_PLAIN_URI);
$dsn = sprintf("mongodb://%s:%s@%s:%d/%s", $username, $password, $parsed["host"], $parsed["port"], $database);
$adminmanager = new MongoDB\Driver\Manager($dsn);

$cmd = array(
    "createUser" => "bugs",
    "roles" => [["role" => "readWrite", "db" => DATABASE_NAME]],
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

$batch = new MongoDB\Driver\WriteBatch();
$batch->insert(array("very" => "important"));
throws(function() use($manager, $batch) {
    $manager->executeWriteBatch(NS, $batch);
}, "MongoDB\Driver\AuthenticationException");

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
OK: Got MongoDB\Driver\AuthenticationException
User deleted
===DONE===
