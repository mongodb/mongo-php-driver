--TEST--
MongoDB\Driver\WriteConcern equality comparison
--FILE--
<?php

$tests = [
    'same w (string)' => [
        'a' => new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY),
        'b' => new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY),
    ],
    'same w (int)' => [
        'a' => new MongoDB\Driver\WriteConcern(1),
        'b' => new MongoDB\Driver\WriteConcern(1),
    ],
    'same default (no w)' => [
        'a' => new MongoDB\Driver\WriteConcern(-2),
        'b' => new MongoDB\Driver\WriteConcern(-2),
    ],
    'same w and j' => [
        'a' => new MongoDB\Driver\WriteConcern(1, 0, true),
        'b' => new MongoDB\Driver\WriteConcern(1, 0, true),
    ],
    'same w and wtimeout' => [
        'a' => new MongoDB\Driver\WriteConcern(1, 1000),
        'b' => new MongoDB\Driver\WriteConcern(1, 1000),
    ],
    'different w (int)' => [
        'a' => new MongoDB\Driver\WriteConcern(1),
        'b' => new MongoDB\Driver\WriteConcern(2),
    ],
    'different w (string)' => [
        'a' => new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY),
        'b' => new MongoDB\Driver\WriteConcern('tag'),
    ],
    'one has w and the other does not' => [
        'a' => new MongoDB\Driver\WriteConcern(1),
        'b' => new MongoDB\Driver\WriteConcern(-2),
    ],
    'different j' => [
        'a' => new MongoDB\Driver\WriteConcern(1, 0, false),
        'b' => new MongoDB\Driver\WriteConcern(1, 0, true),
    ],
    'different wtimeout' => [
        'a' => new MongoDB\Driver\WriteConcern(1, 500),
        'b' => new MongoDB\Driver\WriteConcern(1, 1000),
    ],
    'Object comparison fallback if one value is not a WriteConcern' => [
        'a' => new MongoDB\Driver\WriteConcern(1),
        'b' => new MongoDB\BSON\Int64('1'),
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

Testing same w (string):
int(0)
int(0)

Testing same w (int):
int(0)
int(0)

Testing same default (no w):
int(0)
int(0)

Testing same w and j:
int(0)
int(0)

Testing same w and wtimeout:
int(0)
int(0)

Testing different w (int):
int(-1)
int(1)

Testing different w (string):
int(-1)
int(1)

Testing one has w and the other does not:
int(1)
int(-1)

Testing different j:
int(-1)
int(1)

Testing different wtimeout:
int(-1)
int(1)

Testing Object comparison fallback if one value is not a WriteConcern:
int(1)
int(1)
===DONE===
