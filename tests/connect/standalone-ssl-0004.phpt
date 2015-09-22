--TEST--
Connect to MongoDB with using SSL and verify the stream
--SKIPIF--
<?php if (defined("HHVM_VERSION_ID")) exit("skip HHVM does not support peer verification"); ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; NEEDS("STANDALONE_SSL"); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$dsn = sprintf("%s/?ssl=true&serverselectiontimeoutms=%d", STANDALONE_SSL, 10);

$SSL_DIR = realpath(__DIR__ . "/" . "./../../scripts/ssl/");
$opts = array(
        "peer_name" => "WRONG PEER NAME",
        "verify_peer" => true,
        "verify_peer_name" => true,
        "allow_self_signed" => false,
        "cafile" => $SSL_DIR . "/ca.pem", /* Defaults to openssl.cafile */
        "ciphers" => "HIGH:!EXPORT:!aNULL@STRENGTH",
);

$manager = new MongoDB\Driver\Manager($dsn, array(), $opts);


echo throws(function() use($manager) {
    $bulk = new MongoDB\Driver\BulkWrite;
    $bulk->insert(array("my" => "value"));
    $retval = $manager->executeBulkWrite(NS, $bulk);
}, "MongoDB\\Driver\\Exception\\SSLConnectionException", "executeBulkWrite"), "\n";


echo "Changing to server\n";
$opts["peer_name"] = "server";
$manager = new MongoDB\Driver\Manager($dsn, array(), $opts);
$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(array("my" => "value"));
$retval = $manager->executeBulkWrite(NS, $bulk);
printf("Inserted: %d\n", $retval->getInsertedCount());



?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\SSLConnectionException thrown from executeBulkWrite
%s
Changing to server
Inserted: 1
===DONE===
