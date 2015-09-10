--TEST--
Connect to MongoDB with using SSL and default options
--SKIPIF--
<?php if (defined("HHVM_VERSION_ID")) exit("skip HHVM does not support interception of SSL connection creation"); ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; NEEDS("STANDALONE_SSL"); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE_SSL, array("ssl" => true, "serverselectiontimeoutms" => 100));


$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(array("my" => "value"));

throws(function() use ($manager, $bulk) {
    $retval = $manager->executeBulkWrite(NS, $bulk);
}, "MongoDB\Driver\Exception\SSLConnectionException");



/* Disable all CA and hostname checks */
$opts = array(
    "ssl" => array(
        "verify_peer" => false,
        "verify_peer_name" => false,
        "allow_self_signed" => true,
    ),
);
$context = stream_context_create($opts);

$uriopts = array("ssl" => true, "serverselectiontimeoutms" => 100);
$ctxopts = array("context" => $context);
$manager = new MongoDB\Driver\Manager(STANDALONE_SSL, $uriopts, $ctxopts);


$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(array("my" => "value"));

$retval = $manager->executeBulkWrite(NS, $bulk);
printf("Inserted %d documents\n", $retval->getInsertedCount());



?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\SSLConnectionException
Inserted 1 documents
===DONE===

