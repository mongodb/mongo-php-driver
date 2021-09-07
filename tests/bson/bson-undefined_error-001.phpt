--TEST--
MongoDB\BSON\Undefined cannot be extended
--FILE--
<?php

class MyUndefined extends MongoDB\BSON\Undefined {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyUndefined %s final class %SMongoDB\BSON\Undefined%S in %s on line %d
