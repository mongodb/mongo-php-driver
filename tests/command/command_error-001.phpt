--TEST--
MongoDB\Driver\Command cannot be extended
--FILE--
<?php

class MyCommand extends MongoDB\Driver\Command {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyCommand %s final class %SMongoDB\Driver\Command%S in %s on line %d
