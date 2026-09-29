--TEST--
MongoDB\Driver\ReadPreference equality comparison
--FILE--
<?php

$tests = [
    'same mode' => [
        'a' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY),
        'b' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY)
    ],
    'same mode and tag sets' => [
        'a' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'east']]),
        'b' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'east']])
    ],
    'same mode and maxStalenessSeconds' => [
        'a' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['maxStalenessSeconds' => 120]),
        'b' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['maxStalenessSeconds' => 120])
    ],
    'different modes' => [
        'a' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY),
        'b' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY)
    ],
    'different tag sets' => [
        'a' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'east']]),
        'b' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'west']])
    ],
    'different maxStalenessSeconds' => [
        'a' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['maxStalenessSeconds' => 120]),
        'b' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['maxStalenessSeconds' => 180])
    ],
    'one has tag sets and the other does not' => [
        'a' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'east']]),
        'b' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY)
    ],
    'one has maxStalenessSeconds and the other does not' => [
        'a' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['maxStalenessSeconds' => 120]),
        'b' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY)
    ],
    'different hedges' => [
        'a' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['hedge' => ['enabled' => false]]),
        'b' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['hedge' => ['enabled' => true]])
    ],
    'one has hedges sets and the other does not' => [
        'a' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['hedge' => ['enabled' => true]]),
        'b' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY)
    ],
    'Object comparison fallback if one value is not a ReadPreference' => [
        'a' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY),
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
--EXPECTF--
Deprecated: MongoDB\Driver\ReadPreference::__construct(): The "hedge" option is deprecated as of MongoDB 8.0 and will be removed in a future release in %s

Deprecated: MongoDB\Driver\ReadPreference::__construct(): The "hedge" option is deprecated as of MongoDB 8.0 and will be removed in a future release in %s

Deprecated: MongoDB\Driver\ReadPreference::__construct(): The "hedge" option is deprecated as of MongoDB 8.0 and will be removed in a future release in %s

Testing same mode:
int(0)
int(0)

Testing same mode and tag sets:
int(0)
int(0)

Testing same mode and maxStalenessSeconds:
int(0)
int(0)

Testing different modes:
int(-1)
int(1)

Testing different tag sets:
int(-1)
int(1)

Testing different maxStalenessSeconds:
int(-1)
int(1)

Testing one has tag sets and the other does not:
int(1)
int(-1)

Testing one has maxStalenessSeconds and the other does not:
int(1)
int(-1)

Testing different hedges:
int(-1)
int(1)

Testing one has hedges sets and the other does not:
int(1)
int(-1)

Testing Object comparison fallback if one value is not a ReadPreference:
int(1)
int(1)
===DONE===
