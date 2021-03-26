--TEST--
MongoDB\Driver\ServerApi::bsonSerialize()
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    new MongoDB\Driver\ServerApi('1'),
    new MongoDB\Driver\ServerApi('1', true),
    new MongoDB\Driver\ServerApi('1', null, true),
    new MongoDB\Driver\ServerApi('1', false, false),
];

foreach ($tests as $test) {
    echo toJSON(fromPHP($test)), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
{ "version" : "1" }
{ "version" : "1", "strict" : true }
{ "version" : "1", "deprecationErrors" : true }
{ "version" : "1", "strict" : false, "deprecationErrors" : false }
===DONE===
