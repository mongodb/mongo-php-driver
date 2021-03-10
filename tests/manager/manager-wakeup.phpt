--TEST--
MongoDB\Driver\Manager: Manager cannot be woken up
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '7.99'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$manager = create_test_manager();

echo throws(function() use ($manager) {
    $manager->__wakeup();
}, MongoDB\Driver\Exception\RuntimeException::class), "\n";

echo throws(function() use ($manager) {
    $manager->__wakeup(1, 2);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\RuntimeException
MongoDB\Driver objects cannot be serialized
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\Manager::__wakeup() expects exactly 0 %r(argument|parameter)%rs, 2 given
===DONE===
