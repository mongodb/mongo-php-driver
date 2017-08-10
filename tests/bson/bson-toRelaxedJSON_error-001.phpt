--TEST--
MongoDB\BSON\toRelaxedExtendedJSON(): BSON decoding exceptions
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

/* We can't really test for bson_iter_init() failure within libbson, since
 * bson_reader_read() already checks that the buffer is at least 5 bytes. */
$tests = [
    '',
    str_repeat(fromJSON('{"x": "y"}'), 2),
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
Could not read document from BSON reader
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Reading document did not exhaust input buffer
===DONE===
