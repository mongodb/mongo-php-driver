--TEST--
MongoDB\BSON\Binary #001 error
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$binary = new MongoDB\BSON\Binary("random binary data", MongoDB\BSON\Binary::TYPE_GENERIC);
$binary->getData(2);
$binary->getType(2);

throws(function() {
    new MongoDB\BSON\Binary("random binary data without type");
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Warning: MongoDB\BSON\Binary::getData() expects exactly 0 parameters, 1 given in %s on line %d

Warning: MongoDB\BSON\Binary::getType() expects exactly 0 parameters, 1 given in %s on line %d
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
===DONE===

