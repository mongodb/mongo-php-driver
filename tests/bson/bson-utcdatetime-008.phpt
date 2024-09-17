--TEST--
MongoDB\BSON\UTCDateTime construction from 64-bit integer as string
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(fn () => new MongoDB\BSON\UTCDateTime('1416445411987'), MongoDB\Driver\Exception\InvalidArgumentException::class), PHP_EOL;

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected integer or object, string given
===DONE===
