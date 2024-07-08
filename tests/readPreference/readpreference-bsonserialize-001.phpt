--TEST--
MongoDB\Driver\ReadPreference::bsonSerialize()
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY_PREFERRED),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY_PREFERRED),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::NEAREST),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY, []),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'ny']]),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'ny'], ['dc' => 'sf', 'use' => 'reporting'], []]),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['maxStalenessSeconds' => 1000]),
];

foreach ($tests as $test) {
    echo toJSON(fromPHP($test)), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
{ "mode" : "primary" }
{ "mode" : "primaryPreferred" }
{ "mode" : "secondary" }
{ "mode" : "secondaryPreferred" }
{ "mode" : "nearest" }
{ "mode" : "primary" }
{ "mode" : "secondary", "tags" : [ { "dc" : "ny" } ] }
{ "mode" : "secondary", "tags" : [ { "dc" : "ny" }, { "dc" : "sf", "use" : "reporting" }, {  } ] }
{ "mode" : "secondary", "maxStalenessSeconds" : 1000 }
===DONE===
