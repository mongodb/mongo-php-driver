--TEST--
Connect to MongoDB with using PLAIN auth mechanism
--XFAIL--
authMechanism=PLAIN (LDAP) tests must be reimplemented (PHPC-1172)
parse_url() tests must be reimplemented (PHPC-1177)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$username = "root";
$password = "toor";
$database = "admin";

$parsed = parse_url(URI);
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
$password = "password";
$database = '$external';

$dsn = sprintf("mongodb://%s:%s@%s:%d/?authSource=%s&authMechanism=PLAIN", $username, $password, $parsed["host"], $parsed["port"], $database);
$manager = new MongoDB\Driver\Manager($dsn);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(array("very" => "important"));
try {
    $manager->executeBulkWrite(NS, $bulk);
    $query = new MongoDB\Driver\Query(array("very" => "important"));
    $cursor = $manager->executeQuery(NS, $query);
    foreach($cursor as $document) {
        var_dump($document->very);
    }
    $cmd = new MongoDB\Driver\Command(array("drop" => COLLECTION_NAME));
    $result = $manager->executeCommand(DATABASE_NAME, $cmd);
} catch(Exception $e) {
    printf("Caught %s: %s\n", get_class($e), $e->getMessage());
}
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
string(9) "important"
User deleted
===DONE===
