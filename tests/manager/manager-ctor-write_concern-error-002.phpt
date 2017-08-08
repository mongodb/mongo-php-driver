--TEST--
MongoDB\Driver\Manager::__construct(): invalid write concern (w range)
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

/* Note: libmongoc does not check w's range in the URI string. 64-bit integers
 * will be truncated by strtol() */

echo throws(function() {
    new MongoDB\Driver\Manager(null, ['w' => 2147483648]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer or string for "w" URI option, 64-bit integer given
===DONE===
