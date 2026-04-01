--TEST--
MongoDB\BSON\ObjectId var_export() (ZEND_PROP_PURPOSE_VAR_EXPORT)
--FILE--
<?php

$oid = new MongoDB\BSON\ObjectId('53e2a1c40640fd72175d4603');

var_export($oid);
echo "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%r\\?%rMongoDB\BSON\ObjectId::__set_state(array(
   'oid' => '53e2a1c40640fd72175d4603',
))
===DONE===