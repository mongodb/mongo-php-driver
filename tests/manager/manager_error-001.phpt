--TEST--
MongoDB\Driver\Manager cannot be extended
--FILE--
<?php

class MyManager extends MongoDB\Driver\Manager {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyManager %s final class %SMongoDB\Driver\Manager%S in %s on line %d
