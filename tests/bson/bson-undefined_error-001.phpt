--TEST--
MongoDB\BSON\Undefined cannot be extended
--FILE--
<?php

class MyUndefined extends MongoDB\BSON\Undefined {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyUndefined may not inherit from final class (MongoDB\BSON\Undefined) in %s on line %d
