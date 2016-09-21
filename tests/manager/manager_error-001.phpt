--TEST--
MongoDB\Driver\Manager cannot be extended
--FILE--
<?php

class MyManager extends MongoDB\Driver\Manager {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyManager may not inherit from final class (MongoDB\Driver\Manager) in %s on line %d
