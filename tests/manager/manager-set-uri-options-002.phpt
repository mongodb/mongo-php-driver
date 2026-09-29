--TEST--
MongoDB\Driver\Manager: Connecting to MongoDB using "ssl" from $options
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_ssl(); ?>
<?php skip_if_not_ssl(); ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$options = array(
    "ssl" => false,
    "serverselectiontimeoutms" => 100,
    'tlsAllowInvalidCertificates' => true,
    'tlsAllowInvalidHostnames' => true,
);
/* The server requires SSL */
$manager = create_test_manager(URI, $options);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(array("my" => "value"));

throws(function() use ($manager, $bulk) {
    $inserted = $manager->executeBulkWrite(NS, $bulk)->getInsertedCount();
    printf("Inserted incorrectly: %d\n", $inserted);
}, MongoDB\Driver\Exception\ConnectionException::class);

// Enable SSL and reconnect
$options['ssl'] = true;
$manager = create_test_manager(URI, $options);
$bulk = new MongoDB\Driver\BulkWrite;

$bulk->insert(array("my" => "value"));
$inserted = $manager->executeBulkWrite(NS, $bulk)->getInsertedCount();
printf("Inserted: %d\n", $inserted);
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\ConnectionException
Inserted: 1
===DONE===
