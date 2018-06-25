--TEST--
MongoDB\Driver\Manager::__construct(): invalid write concern (wtimeoutms range exceeds INT32_MAX)
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

/* Note: libmongoc does not check wTimeoutMS's upper bounds in the URI string.
 * 64-bit integers will be truncated by strtol(). */

echo throws(function() {
    new MongoDB\Driver\Manager(null, ['wTimeoutMS' => 2147483648]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "wTimeoutMS" URI option, 64-bit integer given
===DONE===
