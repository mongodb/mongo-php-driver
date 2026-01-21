--TEST--
MongoDB\BSON\Binary vector methods argument count errors
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '7.99'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$bv = MongoDB\BSON\Binary::fromVector([1, 2, 3, 4], MongoDB\BSON\VectorType::Int8);

echo throws(function() use ($bv) {
    $bv->toArray(2);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() use ($bv) {
    $bv->getVectorType(2);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    MongoDB\BSON\Binary::fromVector();
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Binary::toArray() expects exactly 0 %r(argument|parameter)%rs, 1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Binary::getVectorType() expects exactly 0 %r(argument|parameter)%rs, 1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Binary::fromVector() expects exactly 2 %r(argument|parameter)%rs, 0 given
===DONE===
