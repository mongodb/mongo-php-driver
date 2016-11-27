--TEST--
MongoDB\Driver\ReadPreference::bsonSerialize()
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY_PREFERRED),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY_PREFERRED),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_NEAREST),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY, []),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, [['dc' => 'ny']]),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, [['dc' => 'ny'], ['dc' => 'sf', 'use' => 'reporting'], []]),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, null, ['maxStalenessSeconds' => 1000]),
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
