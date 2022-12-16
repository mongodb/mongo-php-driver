--TEST--
MongoDB\BSON\Document::fromBSON(): BSON decoding exceptions
--DESCRIPTION--
This test was adapted from bson-toPHP_error-002.phpt
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* We can't really test for bson_iter_init() failure within libbson, since
 * bson_reader_read() already checks that the buffer is at least 5 bytes. */
$invalidBson = [
    '',
    str_repeat(fromJSON('{"x": "y"}'), 2),
];

foreach ($invalidBson as $bson) {
    echo throws(function () use ($bson) {
        MongoDB\BSON\Document::fromBSON($bson);
    }, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Could not read document from BSON reader
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Reading document did not exhaust input buffer
===DONE===
