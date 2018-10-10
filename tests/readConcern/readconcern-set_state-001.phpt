--TEST--
MongoDB\Driver\ReadConcern::__set_state()
--FILE--
<?php

$tests = [
    MongoDB\Driver\ReadConcern::AVAILABLE,
    MongoDB\Driver\ReadConcern::LINEARIZABLE,
    MongoDB\Driver\ReadConcern::LOCAL,
    MongoDB\Driver\ReadConcern::MAJORITY,
];

foreach ($tests as $level) {
    var_export(MongoDB\Driver\ReadConcern::__set_state([
        'level' => $level,
    ]));
    echo "\n\n";
}

/* Test with level unset */
var_export(MongoDB\Driver\ReadConcern::__set_state([
]));
echo "\n\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
MongoDB\Driver\ReadConcern::__set_state(array(
%w'level' => 'available',
))

MongoDB\Driver\ReadConcern::__set_state(array(
%w'level' => 'linearizable',
))

MongoDB\Driver\ReadConcern::__set_state(array(
%w'level' => 'local',
))

MongoDB\Driver\ReadConcern::__set_state(array(
%w'level' => 'majority',
))

MongoDB\Driver\ReadConcern::__set_state(array(
))

===DONE===
