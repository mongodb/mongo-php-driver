--TEST--
BSON\toJSON(): BSON decoding exceptions
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

/* We can't really test for bson_iter_init() failure, since bson_reader_read()
 * already checks that the buffer is at least 5 bytes.
 */
$invalidBson = array(
    '',
    str_repeat(fromJSON('{"x": "y"}'), 2),
);

foreach ($invalidBson as $bson) {
    echo throws(function() use ($bson) {
        toJSON($bson);
    }, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Could not read document from BSON reader
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Reading document did not exhaust input buffer
===DONE===
