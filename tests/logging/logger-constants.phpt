--TEST--
MongoDB\Driver\Logging\Logger constants
--FILE--
<?php

use MongoDB\Driver\Logging\Logger;

var_dump(Logger::LEVEL_ERROR);
var_dump(Logger::LEVEL_CRITICAL);
var_dump(Logger::LEVEL_WARNING);
var_dump(Logger::LEVEL_MESSAGE);
var_dump(Logger::LEVEL_INFO);
var_dump(Logger::LEVEL_DEBUG);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
int(0)
int(1)
int(2)
int(3)
int(4)
int(5)
===DONE===
