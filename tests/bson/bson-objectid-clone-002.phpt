--TEST--
MongoDB\BSON\ObjectId can be cloned (PHP >= 8.2)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.2'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$objectId = new MongoDB\BSON\ObjectId("53e2a1c40640fd72175d4603");
$clone = clone $objectId;

var_dump($clone == $objectId);
var_dump($clone === $objectId);

unset($objectId);

var_dump($clone);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(false)
object(MongoDB\BSON\ObjectId)#%d (1) {
  ["oid"]=>
  string(24) "53e2a1c40640fd72175d4603"
}
===DONE===
