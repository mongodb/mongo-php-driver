--TEST--
MongoDB\Driver\WriteConcern: var_export()
--FILE--
<?php

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
    // Note: wtimeout is only applicable for w > 1
    new MongoDB\Driver\WriteConcern(-2, 1000),
    // 64-bit wtimeout may be reported as integer or string
    MongoDB\Driver\WriteConcern::__set_state(['w' => 2, 'wtimeout' => '2147483648']),
];

foreach ($tests as $test) {
    echo var_export($test, true), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
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
MongoDB\Driver\WriteConcern::__set_state(array(
   'w' => 2,
   'wtimeout' => %r2147483648|'2147483648'%r,
))
===DONE===
