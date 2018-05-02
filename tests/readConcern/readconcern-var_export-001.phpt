--TEST--
MongoDB\Driver\ReadConcern: var_export()
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
    echo var_export($test, true), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
MongoDB\Driver\ReadConcern::__set_state(array(
))
MongoDB\Driver\ReadConcern::__set_state(array(
   'level' => 'linearizable',
))
MongoDB\Driver\ReadConcern::__set_state(array(
   'level' => 'local',
))
MongoDB\Driver\ReadConcern::__set_state(array(
   'level' => 'majority',
))
MongoDB\Driver\ReadConcern::__set_state(array(
   'level' => 'available',
))
===DONE===
