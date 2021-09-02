--TEST--
MongoDB\Driver\Server cannot be extended
--FILE--
<?php

class MyServer extends MongoDB\Driver\Server {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyServer %s final class %SMongoDB\Driver\Server%S in %s on line %d
