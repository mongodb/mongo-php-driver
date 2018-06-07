--TEST--
MongoDB\Driver\WriteConcern::bsonSerialize()
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    new MongoDB\Driver\WriteConcern(-3), // MONGOC_WRITE_CONCERN_W_MAJORITY
    new MongoDB\Driver\WriteConcern(-2), // MONGOC_WRITE_CONCERN_W_DEFAULT
    new MongoDB\Driver\WriteConcern(-1),
    new MongoDB\Driver\WriteConcern(0),
    new MongoDB\Driver\WriteConcern(1),
    new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY),
    new MongoDB\Driver\WriteConcern('tag'),
    new MongoDB\Driver\WriteConcern(1, 0),
    new MongoDB\Driver\WriteConcern(1, 0, false),
    new MongoDB\Driver\WriteConcern(1, 1000),
    new MongoDB\Driver\WriteConcern(1, 1000, true),
    new MongoDB\Driver\WriteConcern(-2, 0, true),
    // Note: wtimeout is only applicable applies for w > 1
    new MongoDB\Driver\WriteConcern(-2, 1000),
];

foreach ($tests as $test) {
    echo var_export($test, true), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
MongoDB\Driver\WriteConcern::__set_state(array(
   'w' => 'majority',
))
MongoDB\Driver\WriteConcern::__set_state(array(
))
MongoDB\Driver\WriteConcern::__set_state(array(
   'w' => -1,
))
MongoDB\Driver\WriteConcern::__set_state(array(
   'w' => 0,
))
MongoDB\Driver\WriteConcern::__set_state(array(
   'w' => 1,
))
MongoDB\Driver\WriteConcern::__set_state(array(
   'w' => 'majority',
))
MongoDB\Driver\WriteConcern::__set_state(array(
   'w' => 'tag',
))
MongoDB\Driver\WriteConcern::__set_state(array(
   'w' => 1,
))
MongoDB\Driver\WriteConcern::__set_state(array(
   'w' => 1,
   'j' => false,
))
MongoDB\Driver\WriteConcern::__set_state(array(
   'w' => 1,
   'wtimeout' => 1000,
))
MongoDB\Driver\WriteConcern::__set_state(array(
   'w' => 1,
   'j' => true,
   'wtimeout' => 1000,
))
MongoDB\Driver\WriteConcern::__set_state(array(
   'j' => true,
))
MongoDB\Driver\WriteConcern::__set_state(array(
   'wtimeout' => 1000,
))
===DONE===
