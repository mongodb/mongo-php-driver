--TEST--
MongoDB\Driver\WriteConcern::__set_state()
--FILE--
<?php

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
    // wtimeout accepts 64-bit integers as strings
    [ 'wtimeout' => '2147483648'],
];

foreach ($tests as $fields) {
    var_export(MongoDB\Driver\WriteConcern::__set_state($fields));
    echo "\n\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%r\\?%rMongoDB\Driver\WriteConcern::__set_state(array(
   'w' => 'majority',
))

%r\\?%rMongoDB\Driver\WriteConcern::__set_state(array(
))

%r\\?%rMongoDB\Driver\WriteConcern::__set_state(array(
   'w' => -1,
))

%r\\?%rMongoDB\Driver\WriteConcern::__set_state(array(
   'w' => 0,
))

%r\\?%rMongoDB\Driver\WriteConcern::__set_state(array(
   'w' => 1,
))

%r\\?%rMongoDB\Driver\WriteConcern::__set_state(array(
   'w' => 'majority',
))

%r\\?%rMongoDB\Driver\WriteConcern::__set_state(array(
   'w' => 'tag',
))

%r\\?%rMongoDB\Driver\WriteConcern::__set_state(array(
   'w' => 1,
   'j' => false,
))

%r\\?%rMongoDB\Driver\WriteConcern::__set_state(array(
   'w' => 1,
   'wtimeout' => 1000,
))

%r\\?%rMongoDB\Driver\WriteConcern::__set_state(array(
   'w' => 1,
   'j' => true,
   'wtimeout' => 1000,
))

%r\\?%rMongoDB\Driver\WriteConcern::__set_state(array(
   'j' => true,
))

%r\\?%rMongoDB\Driver\WriteConcern::__set_state(array(
   'wtimeout' => 1000,
))

%r\\?%rMongoDB\Driver\WriteConcern::__set_state(array(
   'wtimeout' => %r2147483648|'2147483648'%r,
))

===DONE===
