--TEST--
MongoDB\Driver\ReadConcern: var_export()
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    new MongoDB\Driver\ReadConcern(),
    new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LOCAL),
];

foreach ($tests as $test) {
    echo var_export($test, true), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%r\\?%rMongoDB\Driver\ReadConcern::__set_state(array(
))
%r\\?%rMongoDB\Driver\ReadConcern::__set_state(array(
   'level' => 'local',
))
===DONE===
