--TEST--
MongoDB\Driver\WriteError cannot be extended
--FILE--
<?php

class MyWriteError extends MongoDB\Driver\WriteError {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyWriteError may not inherit from final class (MongoDB\Driver\WriteError) in %s on line %d
