--TEST--
MongoDB\Driver\WriteConcern::__set_state() with 64-bit wtimeoutms
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

var_export(MongoDB\Driver\WriteConcern::__set_state([ 'wtimeout' => '2147483648' ]));
echo "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
MongoDB\Driver\WriteConcern::__set_state(array(
   'wtimeout' => '2147483648',
))
===DONE===
