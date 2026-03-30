--TEST--
MongoDB\Driver\ReadPreference equality comparison
--FILE--
<?php

// True: same mode
var_dump(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY) == new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY));
// True: same mode and tag sets
var_dump(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'east']]) == new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'east']]));
// True: same mode and maxStalenessSeconds
var_dump(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['maxStalenessSeconds' => 120]) == new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['maxStalenessSeconds' => 120]));

// False for different modes
var_dump(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY) == new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY));
// False for different tag sets
var_dump(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'east']]) == new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'west']]));
// False for different maxStalenessSeconds
var_dump(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['maxStalenessSeconds' => 120]) == new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['maxStalenessSeconds' => 180]));
// False if one has tag sets and the other does not
var_dump(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'east']]) == new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY));
// False if one has maxStalenessSeconds and the other does not
var_dump(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['maxStalenessSeconds' => 120]) == new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY));
// False for different hedges
var_dump(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['hedge' => ['enabled' => false]]) == new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['hedge' => ['enabled' => true]]));
// False if one has hedges sets and the other does not
var_dump(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['hedge' => ['enabled' => true]]) == new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY));

// Object comparison fallback if one value is not a ReadPreference
var_dump(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY) == new MongoDB\BSON\Int64('1'));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
%A
bool(false)
%A
bool(false)
bool(false)
===DONE===
