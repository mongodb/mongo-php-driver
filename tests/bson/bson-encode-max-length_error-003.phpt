--TEST--
PHPC-2741: String hint option rejects values larger than INT32_MAX
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_enough_memory(0x80000000); ?>
--INI--
memory_limit=-1
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* The string "hint" option reaches bson_append_utf8 through option helpers in
 * Query and BulkWrite, which narrow Z_STRLEN_P (size_t) to int. Before the
 * bound check a 4 GiB hint wrapped to zero and appended an empty string with no
 * error. */
$tooLong = str_repeat('a', 0x80000000);

echo throws(function() use ($tooLong) {
    new MongoDB\Driver\Query([], ['hint' => $tooLong]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($tooLong) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->update(['x' => 1], ['$set' => ['x' => 2]], ['hint' => $tooLong]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "hint" option to be <= 2147483647 bytes, 2147483648 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "hint" option to be <= 2147483647 bytes, 2147483648 given
===DONE===
