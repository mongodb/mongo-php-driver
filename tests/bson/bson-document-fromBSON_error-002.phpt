--TEST--
MongoDB\BSON\Document::fromBSON(): BSON decoding exceptions for malformed documents
--DESCRIPTION--
This test was adapted from bson-toPHP_error-003.phpt
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    pack('Vx', 4), // Empty document with invalid length (too small)
    pack('Vx', 6), // Empty document with invalid length (too large)
];

foreach ($tests as $bson) {
    echo throws(function() use ($bson) {
        MongoDB\BSON\Document::fromBSON($bson);
    }, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Could not read document from BSON reader
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Could not read document from BSON reader
===DONE===
