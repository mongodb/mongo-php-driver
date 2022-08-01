--TEST--
MongoDB\Driver\ReadConcern::__set_state()
--FILE--
<?php

$tests = [
    MongoDB\Driver\ReadConcern::AVAILABLE,
    MongoDB\Driver\ReadConcern::LINEARIZABLE,
    MongoDB\Driver\ReadConcern::LOCAL,
    MongoDB\Driver\ReadConcern::MAJORITY,
    MongoDB\Driver\ReadConcern::SNAPSHOT,
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
%r\\?%rMongoDB\Driver\ReadConcern::__set_state(array(
%w'level' => 'available',
))

%r\\?%rMongoDB\Driver\ReadConcern::__set_state(array(
%w'level' => 'linearizable',
))

%r\\?%rMongoDB\Driver\ReadConcern::__set_state(array(
%w'level' => 'local',
))

%r\\?%rMongoDB\Driver\ReadConcern::__set_state(array(
%w'level' => 'majority',
))

%r\\?%rMongoDB\Driver\ReadConcern::__set_state(array(
%w'level' => 'snapshot',
))

%r\\?%rMongoDB\Driver\ReadConcern::__set_state(array(
))

===DONE===
