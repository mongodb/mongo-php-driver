--TEST--
MongoDB\Driver\WriteConcern::__set_state()
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    [ 'w' => -3 ],
    [ 'w' => -2 ], // -2 is default
    [ 'w' => -1 ],
    [ 'w' => 0 ],
    [ 'w' => 1 ],
    [ 'w' => 'majority' ],
    [ 'w' => 'tag' ],
    [ 'w' => 1, 'j' => false ],
    [ 'w' => 1, 'wtimeout' => 1000 ],
    [ 'w' => 1, 'j' => true, 'wtimeout' => 1000 ],
    [ 'j' => true ],
    [ 'wtimeout' => 1000 ],
];

foreach ($tests as $fields) {
    var_export(MongoDB\Driver\WriteConcern::__set_state($fields));
    echo "\n\n";
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
