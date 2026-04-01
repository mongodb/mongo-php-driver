--TEST--
MongoDB\BSON\ObjectId array cast (ZEND_PROP_PURPOSE_ARRAY_CAST)
--FILE--
<?php

$oid = new MongoDB\BSON\ObjectId('53e2a1c40640fd72175d4603');

var_dump((array) $oid);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(1) {
  ["oid"]=>
  string(24) "53e2a1c40640fd72175d4603"
}
===DONE===