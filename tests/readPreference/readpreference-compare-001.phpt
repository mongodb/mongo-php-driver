--TEST--
MongoDB\Driver\ReadPreference equality comparison
--FILE--
<?php

$tests = [
    'same mode' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY) == new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY),
    'same mode and tag sets' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'east']]) == new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'east']]),
    'same mode and maxStalenessSeconds' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['maxStalenessSeconds' => 120]) == new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['maxStalenessSeconds' => 120]),
    'different modes' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY) == new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY),
    'different tag sets' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'east']]) == new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'west']]),
    'different maxStalenessSeconds' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['maxStalenessSeconds' => 120]) == new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['maxStalenessSeconds' => 180]),
    'one has tag sets and the other does not' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'east']]) == new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY),
    'one has maxStalenessSeconds and the other does not' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['maxStalenessSeconds' => 120]) == new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY),
    'different hedges' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['hedge' => ['enabled' => false]]) == new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['hedge' => ['enabled' => true]]),
    'one has hedges sets and the other does not' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['hedge' => ['enabled' => true]]) == new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY),
    'Object comparison fallback if one value is not a ReadPreference' => new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY) == new MongoDB\BSON\Int64('1'),
];

foreach ($tests as $test => $result) {
    echo "Testing $test: ";
    var_dump($result);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: MongoDB\Driver\ReadPreference::__construct(): The "hedge" option is deprecated as of MongoDB 8.0 and will be removed in a future release in %s

Deprecated: MongoDB\Driver\ReadPreference::__construct(): The "hedge" option is deprecated as of MongoDB 8.0 and will be removed in a future release in %s

Deprecated: MongoDB\Driver\ReadPreference::__construct(): The "hedge" option is deprecated as of MongoDB 8.0 and will be removed in a future release in %s
Testing same mode: bool(true)
Testing same mode and tag sets: bool(true)
Testing same mode and maxStalenessSeconds: bool(true)
Testing different modes: bool(false)
Testing different tag sets: bool(false)
Testing different maxStalenessSeconds: bool(false)
Testing one has tag sets and the other does not: bool(false)
Testing one has maxStalenessSeconds and the other does not: bool(false)
Testing different hedges: bool(false)
Testing one has hedges sets and the other does not: bool(false)
Testing Object comparison fallback if one value is not a ReadPreference: bool(false)
===DONE===
