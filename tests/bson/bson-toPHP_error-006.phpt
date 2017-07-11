--TEST--
MongoDB\BSON\toPHP(): BSON decoding throws multiple exceptions
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    // two undefined fields in root document
    pack('VCa*xCa*xx', 13, 0x06, 'u1', 0x06, 'u2'),
    // undefined field and symbol field in root document
    pack('VCa*xCa*xVa*xx', 21, 0x06, 'u1', 0x0E, 's1', 4, 'foo'),
    // two undefined fields in root (first) and embedded (second) documents
    pack('VCa*xCa*xVCa*xxx', 22, 0x06, 'u1', 0x03, 'e1', 9, 0x06, 'u2'),
    // two undefined fields in embedded (first) and root (second) documents
    pack('VCa*xVCa*xxCa*xx', 22, 0x03, 'e1', 9, 0x06, 'u1', 0x06, 'u2'),
    // two undefined fields in separate embedded documents
    pack('VCa*xVCa*xxCa*xVCa*xxx', 31, 0x03, 'e1', 9, 0x06, 'u1', 0x03, 'e2', 9, 0x06, 'u2'),
];

foreach ($tests as $bson) {
    try {
        var_dump(toPHP($bson));
    } catch (MongoDB\Driver\Exception\UnexpectedValueException $e) {
        do {
            printf("OK: %s\n%s\n", get_class($e), $e->getMessage());
        } while ($e = $e->getPrevious());
    }

    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: MongoDB\Driver\Exception\UnexpectedValueException
Detected unsupported BSON type 0x06 (undefined) for fieldname "u2"
OK: MongoDB\Driver\Exception\UnexpectedValueException
Detected unsupported BSON type 0x06 (undefined) for fieldname "u1"

OK: MongoDB\Driver\Exception\UnexpectedValueException
Detected unsupported BSON type 0x0E (symbol) for fieldname "s1"
OK: MongoDB\Driver\Exception\UnexpectedValueException
Detected unsupported BSON type 0x06 (undefined) for fieldname "u1"

OK: MongoDB\Driver\Exception\UnexpectedValueException
Detected unsupported BSON type 0x06 (undefined) for fieldname "u2"
OK: MongoDB\Driver\Exception\UnexpectedValueException
Detected unsupported BSON type 0x06 (undefined) for fieldname "u1"

OK: MongoDB\Driver\Exception\UnexpectedValueException
Detected unsupported BSON type 0x06 (undefined) for fieldname "u2"
OK: MongoDB\Driver\Exception\UnexpectedValueException
Detected unsupported BSON type 0x06 (undefined) for fieldname "u1"

OK: MongoDB\Driver\Exception\UnexpectedValueException
Detected unsupported BSON type 0x06 (undefined) for fieldname "u2"
OK: MongoDB\Driver\Exception\UnexpectedValueException
Detected unsupported BSON type 0x06 (undefined) for fieldname "u1"

===DONE===
