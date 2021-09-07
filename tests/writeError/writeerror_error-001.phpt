--TEST--
MongoDB\Driver\WriteError cannot be extended
--FILE--
<?php

class MyWriteError extends MongoDB\Driver\WriteError {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyWriteError %s final class %SMongoDB\Driver\WriteError%S in %s on line %d
