--TEST--
PHPC-2741: Encoding rejects string and binary values larger than INT32_MAX
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_enough_memory(0x80000000); ?>
--INI--
memory_limit=-1
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* One byte beyond the largest length BSON can encode as an int32. The payload
 * is built once and shared, since each copy costs two gigabytes. */
$tooLong = str_repeat('a', 0x80000000);

echo throws(function() use ($tooLong) {
    MongoDB\BSON\Document::fromPHP(['string' => $tooLong]);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

echo throws(function() use ($tooLong) {
    new MongoDB\BSON\Binary($tooLong, MongoDB\BSON\Binary::TYPE_GENERIC);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($tooLong) {
    MongoDB\BSON\Symbol::__set_state(['symbol' => $tooLong]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($tooLong) {
    new MongoDB\Driver\Query([], ['comment' => $tooLong]);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Expected string for field path "string" to be <= 2147483647 bytes, 2147483648 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected data length to be <= 2147483647 bytes, 2147483648 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected symbol length to be <= 2147483647 bytes, 2147483648 given
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Expected string to be <= 2147483647 bytes, 2147483648 given
===DONE===
