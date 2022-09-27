--TEST--
MongoDB\Driver\ReadConcern::__set_state()
--FILE--
<?php

var_export(MongoDB\Driver\ReadConcern::__set_state([
    'level' => MongoDB\Driver\ReadConcern::AVAILABLE,
]));
echo "\n\n";

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
))

===DONE===
