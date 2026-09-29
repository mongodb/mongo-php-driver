--TEST--
MongoDB\Driver\ReadConcern equality comparison
--FILE--
<?php

$tests = [
    'same level' => [
        'a' => new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LOCAL),
        'b' => new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LOCAL)
    ],
    'same default (no level)' => [
        'a' => new MongoDB\Driver\ReadConcern(),
        'b' => new MongoDB\Driver\ReadConcern()
    ],
    'different levels' => [
        'a' => new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LOCAL),
        'b' => new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::MAJORITY)
    ],
    'one has level and the other does not' => [
        'a' => new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LOCAL),
        'b' => new MongoDB\Driver\ReadConcern()
    ],
    'Object comparison fallback if one value is not a ReadConcern' => [
        'a' => new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LOCAL),
        'b' => new MongoDB\BSON\Int64('1')
    ],
];

foreach ($tests as $name => $test) {
    echo "\nTesting $name:\n";
    var_dump($test['a'] <=> $test['b']);
    var_dump($test['b'] <=> $test['a']);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--

Testing same level:
int(0)
int(0)

Testing same default (no level):
int(0)
int(0)

Testing different levels:
int(-1)
int(1)

Testing one has level and the other does not:
int(1)
int(-1)

Testing Object comparison fallback if one value is not a ReadConcern:
int(1)
int(1)
===DONE===
