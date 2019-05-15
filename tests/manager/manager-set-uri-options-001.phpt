--TEST--
MongoDB\Driver\Manager: Logging into MongoDB using credentials from $options
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_auth(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$url = parse_url(URI);
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

echo throws(function() use ($manager, $bulk) {
    $inserted = $manager->executeBulkWrite(NS, $bulk)->getInsertedCount();
    printf("Incorrectly inserted: %d\n", $inserted);
}, 'MongoDB\Driver\Exception\BulkWriteException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Inserted: 1
OK: Got MongoDB\Driver\Exception\BulkWriteException
Bulk write failed due to previous MongoDB\Driver\Exception\AuthenticationException: Authentication failed.
===DONE===
