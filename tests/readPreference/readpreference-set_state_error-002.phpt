--TEST--
MongoDB\Driver\ReadPreference::__set_state() requires correct data types and values
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    MongoDB\Driver\ReadPreference::__set_state(['mode' => 'secondary', 'maxStalenessSeconds' => 2147483648]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ReadPreference initialization requires "maxStalenessSeconds" integer field to be <= 2147483647
===DONE===
