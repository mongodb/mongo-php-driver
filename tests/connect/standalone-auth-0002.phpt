--TEST--
Connect to MongoDB with using default auth mechanism #002
--XFAIL--
parse_url() tests must be reimplemented (PHPC-1177)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_auth(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$username = "root";
$password = "tooring";
$database = "admin";

$parsed = parse_url(URI);
$dsn = sprintf("mongodb://%s:%s@%s:%d/%s", $username, $password, $parsed["host"], $parsed["port"], $database);
$manager = new MongoDB\Driver\Manager($dsn);

$bulk = new MongoDB\Driver\BulkWrite;

$bulk->insert(array("my" => "value"));
throws(function() use($manager, $bulk) {
    $retval = $manager->executeBulkWrite(NS, $bulk);
}, "MongoDB\Driver\Exception\AuthenticationException");

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\AuthenticationException
===DONE===
