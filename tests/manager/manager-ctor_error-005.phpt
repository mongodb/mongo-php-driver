--TEST--
MongoDB\Driver\Manager::__construct(): Invalid handshake data
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$tests = [
    (object) [],
    'string',
    ['name' => []],
    ['version' => []],
    ['platform' => []],
];

foreach ($tests as $driver) {
    echo throws(function () use ($driver) {
        $manager = create_test_manager(null, [], ['driver' => $driver]);
    }, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "driver" driver option to be an array, stdClass given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "driver" driver option to be an array, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "name" handshake option to be a string, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "version" handshake option to be a string, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "platform" handshake option to be a string, array given
===DONE===
