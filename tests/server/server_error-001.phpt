--TEST--
MongoDB\Driver\Server cannot be extended
--FILE--
<?php

class MyServer extends MongoDB\Driver\Server {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyServer may not inherit from final class (MongoDB\Driver\Server) in %s on line %d
