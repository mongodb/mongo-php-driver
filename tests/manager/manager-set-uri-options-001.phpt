--TEST--
MongoDB\Driver\Manager: Logging into MongoDB using credentials from $options
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; NEEDS("STANDALONE_AUTH"); ?>
<?php CLEANUP(STANDALONE_AUTH) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$url = parse_url(STANDALONE_AUTH);
parse_str($url["query"], $args);


$dsn = sprintf("mongodb://%s:%d/", $url["host"], $url["port"]);
$options = array(
    "username" => $url["user"],
    "password" => $url["pass"],
) + $args;
$manager = new MongoDB\Driver\Manager($dsn, $options);


$bulk = new MongoDB\Driver\BulkWrite;

$bulk->insert(array("my" => "value"));
$inserted = $manager->executeBulkWrite(NS, $bulk)->getInsertedCount();

printf("Inserted: %d\n", $inserted);


$options["username"]      = "not-found-user";
$manager = new MongoDB\Driver\Manager($dsn, $options);
$bulk = new MongoDB\Driver\BulkWrite;

$bulk->insert(array("my" => "value"));
throws(function() use ($manager, $bulk) {
    $inserted = $manager->executeBulkWrite(NS, $bulk)->getInsertedCount();
    printf("Incorrectly inserted: %d\n", $inserted);
}, "MongoDB\Driver\Exception\AuthenticationException");
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Inserted: 1
OK: Got MongoDB\Driver\Exception\AuthenticationException
===DONE===
