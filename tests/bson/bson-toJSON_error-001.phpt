--TEST--
MongoDB\BSON\toJSON(): BSON decoding exceptions
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* We can't really test for bson_iter_init() failure within libbson, since
 * bson_reader_read() already checks that the buffer is at least 5 bytes. */
$tests = [
    '',
    str_repeat(MongoDB\BSON\fromJSON('{"x": "y"}'), 2),
];

foreach ($tests as $bson) {
    echo throws(function() use ($bson) {
        MongoDB\BSON\toJSON($bson);
    }, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toJSON() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Could not read document from BSON reader

Deprecated: Function MongoDB\BSON\toJSON() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Reading document did not exhaust input buffer
===DONE===
