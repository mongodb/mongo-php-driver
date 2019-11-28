--TEST--
MongoDB\Driver\WriteConcern::getWtimeout() emits warning on truncation of 64-bit value
--SKIPIF--
<?php if (4 !== PHP_INT_SIZE) { die('skip Only for 32-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$manager = new MongoDB\Driver\Manager('mongodb://127.0.0.1/?w=2&wtimeoutms=4294967296');

echo raises(function() use ($manager) {
    var_dump($manager->getWriteConcern()->getWtimeout());
}, E_WARNING), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got E_WARNING
Truncating 64-bit value for wTimeoutMS
===DONE===
