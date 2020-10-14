--TEST--
MongoDB\Driver\Manager: Manager cannot be woken up
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '7.99'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$manager = new MongoDB\Driver\Manager();
throws(function() use($manager) {
    $manager->__wakeup();
}, "MongoDB\Driver\Exception\RuntimeException");

$manager->__wakeup(1, 2);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\RuntimeException

Warning: MongoDB\Driver\Manager::__wakeup() expects exactly 0 parameters, 2 given in %s on line %d
===DONE===
