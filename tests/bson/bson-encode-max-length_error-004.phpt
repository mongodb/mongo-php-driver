--TEST--
PHPC-2741: Encoding rejects field keys larger than INT32_MAX
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_enough_memory(0x80000000); ?>
--INI--
memory_limit=-1
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* A key longer than INT32_MAX wraps its size_t/long length to a small
 * non-negative int in the libbson append, silently truncating the field name
 * to a prefix (e.g. a key of 2^32 + 4 bytes starting with "root" encodes as
 * { "root": ... }). Reject it before the narrowing. The key is deliberately
 * not embedded in the message, as it may be gigabytes. */
$tooLong = str_repeat('a', 0x80000000);

echo throws(function() use ($tooLong) {
    MongoDB\BSON\Document::fromPHP([$tooLong => 1]);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Expected key length to be <= 2147483647 bytes, 2147483648 given
===DONE===
