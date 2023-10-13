--TEST--
MongoDB\Driver\Query::__construct() prohibits PackedArray for document values
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    new MongoDB\Driver\Query(MongoDB\BSON\PackedArray::fromPHP([]));
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

$tests = [
    ['collation' => MongoDB\BSON\PackedArray::fromPHP([])],
    ['let' => MongoDB\BSON\PackedArray::fromPHP([])],
    ['max' => MongoDB\BSON\PackedArray::fromPHP([])],
    ['min' => MongoDB\BSON\PackedArray::fromPHP([])],
    ['projection' => MongoDB\BSON\PackedArray::fromPHP([])],
    ['sort' => MongoDB\BSON\PackedArray::fromPHP([])],
    ['modifiers' => ['$max' => MongoDB\BSON\PackedArray::fromPHP([])]],
    ['modifiers' => ['$min' => MongoDB\BSON\PackedArray::fromPHP([])]],
    ['modifiers' => ['$orderby' => MongoDB\BSON\PackedArray::fromPHP([])]],
];

foreach ($tests as $options) {
    echo throws(function() use ($options) {
        new MongoDB\Driver\Query([], $options);
    }, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
MongoDB\BSON\PackedArray cannot be serialized as a root document
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
MongoDB\BSON\PackedArray cannot be serialized as a root document
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
MongoDB\BSON\PackedArray cannot be serialized as a root document
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
MongoDB\BSON\PackedArray cannot be serialized as a root document
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
MongoDB\BSON\PackedArray cannot be serialized as a root document
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
MongoDB\BSON\PackedArray cannot be serialized as a root document
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
MongoDB\BSON\PackedArray cannot be serialized as a root document
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
MongoDB\BSON\PackedArray cannot be serialized as a root document
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
MongoDB\BSON\PackedArray cannot be serialized as a root document
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
MongoDB\BSON\PackedArray cannot be serialized as a root document
===DONE===
