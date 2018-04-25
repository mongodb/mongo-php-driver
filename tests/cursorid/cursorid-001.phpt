--TEST--
MongoDB\Driver\CursorID BSON serialization
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 2]);
$bulk->insert(['_id' => 3]);
$manager->executeBulkWrite(NS, $bulk);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([], ['batchSize' => 2]));
$cursorId = $cursor->getId();

hex_dump(fromPHP(['cid' => $cursorId]));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
     0 : 12 00 00 00 12 63 69 64 00 %x %x %x %x %x %x %x  [.....cid.%s]
    10 : %x 00                                            [%s.]
===DONE===
