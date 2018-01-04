--TEST--
MongoDB\Driver\ReadConcern::bsonSerialize()
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    new MongoDB\Driver\ReadConcern(),
    new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LINEARIZABLE),
    new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LOCAL),
    new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::MAJORITY),
    new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::AVAILABLE),
];

foreach ($tests as $test) {
    echo toJSON(fromPHP($test)), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
{ }
{ "level" : "linearizable" }
{ "level" : "local" }
{ "level" : "majority" }
{ "level" : "available" }
===DONE===
