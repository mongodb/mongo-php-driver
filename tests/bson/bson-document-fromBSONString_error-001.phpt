--TEST--
MongoDB\BSON\Document::fromBSONString(): BSON decoding exceptions
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* We can't really test for bson_iter_init() failure within libbson, since
 * bson_reader_read() already checks that the buffer is at least 5 bytes. */
$invalidBson = array(
    '',
    str_repeat(fromJSON('{"x": "y"}'), 2),
);

foreach ($invalidBson as $bson) {
    echo throws(function () use ($bson) {
        MongoDB\BSON\Document::fromBSONString($bson);
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
