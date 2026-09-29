--TEST--
MongoDB\Driver\Manager::__construct(): empty authSource option causes error
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    ['mongodb://username@127.0.0.1/?authSource=', []],
    [null, ['authSource' => '']],
];

foreach ($tests as $test) {
    [$uri, $options] = $test;

    echo throws(function() use ($uri, $options) {
        new MongoDB\Driver\Manager($uri, $options);
    }, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://username@127.0.0.1/?authSource='. authSource may not be specified as an empty string.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: authSource may not be specified as an empty string
===DONE===
