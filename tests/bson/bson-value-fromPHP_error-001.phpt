--TEST--
MongoDB\BSON\Value::fromPHP() unsupported types
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$memory = fopen('php://memory', 'r+');
$tests = [
    'resource' => fopen('php://memory', 'r+'),
];

foreach ($tests as $name => $value) {
    printf("Testing %s:\n", $name);
    echo throws(function() use ($value) {
        $bsonValue = MongoDB\BSON\Value::fromPHP($value);
    }, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";
}

fclose($memory);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing resource:
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Unsupported type resource
===DONE===
