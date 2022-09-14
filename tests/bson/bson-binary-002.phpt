--TEST--
MongoDB\BSON\Binary::__construct() defaults to TYPE_GENERIC
--FILE--
<?php

$binaryWithDefaultType = new MongoDB\BSON\Binary('randomBinaryString');
var_dump($binaryWithDefaultType->getData() === 'randomBinaryString');
var_dump($binaryWithDefaultType->getType() === MongoDB\BSON\Binary::TYPE_GENERIC);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
===DONE===
