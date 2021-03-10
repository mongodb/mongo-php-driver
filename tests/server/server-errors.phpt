--TEST--
MongoDB\Driver\Server argument count errors
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '7.99'); ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$server = $manager->executeQuery(NS, new MongoDB\Driver\Query(array()))->getServer();

$methods = [
    'getHost',
    'getTags',
    'getInfo',
    'getLatency',
    'getPort',
    'getType',
    'isPrimary',
    'isSecondary',
    'isArbiter',
    'isHidden',
    'isPassive',
];

foreach ($methods as $method) {
    echo throws(function() use ($server, $method) {
        $server->{$method}(true);
    }, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\Server::getHost() expects exactly 0 %r(argument|parameter)%rs, 1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\Server::getTags() expects exactly 0 %r(argument|parameter)%rs, 1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\Server::getInfo() expects exactly 0 %r(argument|parameter)%rs, 1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\Server::getLatency() expects exactly 0 %r(argument|parameter)%rs, 1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\Server::getPort() expects exactly 0 %r(argument|parameter)%rs, 1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\Server::getType() expects exactly 0 %r(argument|parameter)%rs, 1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\Server::isPrimary() expects exactly 0 %r(argument|parameter)%rs, 1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\Server::isSecondary() expects exactly 0 %r(argument|parameter)%rs, 1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\Server::isArbiter() expects exactly 0 %r(argument|parameter)%rs, 1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\Server::isHidden() expects exactly 0 %r(argument|parameter)%rs, 1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\Server::isPassive() expects exactly 0 %r(argument|parameter)%rs, 1 given
===DONE===
