--TEST--
MongoDB\Driver\Manager: mongodb.debug=stdout
--SKIPIF--
<?php if (defined("HHVM_VERSION_ID")) exit("skip HHVM uses HHVM's logging functionality"); ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--INI--
mongodb.debug=stdout
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);
$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(array('_id' => 1, 'x' => 1));
$result = $manager->executeBulkWrite(NS, $bulk);

ini_set("mongodb.debug", "off");
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%a
[%s]     PHONGO: TRACE   > ENTRY: php_phongo_make_mongo_client():%d
[%s]     PHONGO: DEBUG   > Creating Manager, phongo-1.%d.%d%S[%s] - mongoc-1.%s(%s), libbson-1.%s(%s), php-%s
%a
===DONE===
