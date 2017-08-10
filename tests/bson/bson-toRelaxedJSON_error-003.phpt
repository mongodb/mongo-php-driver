--TEST--
MongoDB\BSON\toRelaxedExtendedJSON(): BSON decoding exceptions for bson_as_canonical_json() failure
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    // Invalid UTF-8 characters (i.e. 0xFE, 0xFF) in field name
    pack('VCCCxVa*xx', 17, 2, 254, 255, 3, 'bar'),
    /* Note: we don't use a three-character string in the underflow case, as
     * the 4-byte string length and payload (i.e. three characters + null byte)
     * coincidentally satisfy the expected size for an 8-byte double. We also
     * don't use a four-character string, since its null byte would be
     * interpreted as the document terminator. The actual document terminator
     * would then remain in the buffer and trigger a "did not exhaust" error.
     */
    pack('VCa*xVa*xx', 17, 1, 'foo', 3, 'ab'), // Invalid field type (underflow)
    pack('VCa*xVa*xx', 20, 1, 'foo', 6, 'abcde'), // Invalid field type (overflow)
];

foreach ($tests as $bson) {
    echo throws(function() use ($bson) {
        toRelaxedExtendedJSON($bson);
    }, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Could not convert BSON document to a JSON string
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Could not convert BSON document to a JSON string
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Could not convert BSON document to a JSON string
===DONE===
