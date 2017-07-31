--TEST--
MongoDB\BSON\BinaryInterface is implemented by MongoDB\BSON\Binary
--FILE--
<?php

$binary = new MongoDB\BSON\Binary('foo', MongoDB\BSON\Binary::TYPE_GENERIC);
var_dump($binary instanceof MongoDB\BSON\BinaryInterface);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===
