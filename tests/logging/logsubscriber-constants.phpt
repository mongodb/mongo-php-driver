--TEST--
MongoDB\Driver\Monitoring\LogSubscriber constants
--FILE--
<?php

use MongoDB\Driver\Monitoring\LogSubscriber;

var_dump(LogSubscriber::LEVEL_ERROR);
var_dump(LogSubscriber::LEVEL_CRITICAL);
var_dump(LogSubscriber::LEVEL_WARNING);
var_dump(LogSubscriber::LEVEL_MESSAGE);
var_dump(LogSubscriber::LEVEL_INFO);
var_dump(LogSubscriber::LEVEL_DEBUG);

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
