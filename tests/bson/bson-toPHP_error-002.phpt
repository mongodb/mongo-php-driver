--TEST--
MongoDB\BSON\toPHP(): BSON decoding exceptions
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* We can't really test for bson_iter_init() failure within libbson, since
 * bson_reader_read() already checks that the buffer is at least 5 bytes. */
$invalidBson = array(
    '',
    str_repeat(MongoDB\BSON\fromJSON('{"x": "y"}'), 2),
);

foreach ($invalidBson as $bson) {
    try {
        var_dump(MongoDB\BSON\toPHP($bson));
    } catch (MongoDB\Driver\Exception\UnexpectedValueException $e) {
        echo $e->getMessage(), "\n";
    }
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
Could not read document from BSON reader

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
Reading document did not exhaust input buffer
===DONE===
