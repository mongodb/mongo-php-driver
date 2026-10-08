--TEST--
PHPC-2741: Javascript, Regex and DBPointer reject values larger than INT32_MAX
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_enough_memory(0x80000000); ?>
--INI--
memory_limit=-1
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* These types reach libbson through appends that take no length argument, so
 * libbson computes strlen() and narrows it internally. Before the bound check
 * the field was silently dropped from the encoded document. */
$tooLong = str_repeat('a', 0x80000000);

echo throws(function() use ($tooLong) {
    new MongoDB\BSON\Javascript($tooLong);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($tooLong) {
    new MongoDB\BSON\Regex($tooLong, '');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($tooLong) {
    new MongoDB\BSON\Regex('', $tooLong);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($tooLong) {
    MongoDB\BSON\DBPointer::__set_state(['ref' => $tooLong, 'id' => '000000000000000000000000']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected code length to be <= 2147483647 bytes, 2147483648 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected pattern length to be <= 2147483647 bytes, 2147483648 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected flags length to be <= 2147483647 bytes, 2147483648 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected ref length to be <= 2147483647 bytes, 2147483648 given
===DONE===
