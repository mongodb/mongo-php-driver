--TEST--
Connect to MongoDB with using default auth mechanism and wrong password
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_auth(); ?>
<?php skip_if_not_auth_mechanism(null); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$username = "root";
$password = "the-wrong-password";
$database = "admin";

$parsed = parse_url(URI);
$dsn = sprintf("mongodb://%s:%s@%s:%d/%s", $username, $password, $parsed["host"], $parsed["port"], $database);
$manager = new MongoDB\Driver\Manager($dsn);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(array("my" => "value"));

echo throws(function() use($manager, $bulk) {
    $manager->executeBulkWrite(NS, $bulk);
}, 'MongoDB\Driver\Exception\BulkWriteException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\BulkWriteException
Bulk write failed due to previous MongoDB\Driver\Exception\AuthenticationException: Authentication failed.
===DONE===
