--TEST--
MongoDB\BSON\toPHP(): BSON decoding exceptions for unsupported BSON types
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    pack('VCa*xx', 10, 0x06, 'foo'), // undefined
    pack('VCa*xVa*xx12x', 37, 0x0C, 'foo', 11, 'collection'), // DBPointer
    pack('VCa*xVa*xx', 18, 0x0E, 'foo', 4, 'bar'), // symbol
];

foreach ($tests as $bson) {
    echo throws(function() use ($bson) {
        var_dump(toPHP($bson));
    }, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected unsupported BSON type 0x06 (undefined) for fieldname "foo"
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected unsupported BSON type 0x0C (DBPointer) for fieldname "foo"
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected unsupported BSON type 0x0E (symbol) for fieldname "foo"
===DONE===
