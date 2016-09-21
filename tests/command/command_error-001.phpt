--TEST--
MongoDB\Driver\Command cannot be extended
--FILE--
<?php

class MyCommand extends MongoDB\Driver\Command {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyCommand may not inherit from final class (MongoDB\Driver\Command) in %s on line %d
