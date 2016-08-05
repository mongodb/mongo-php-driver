--TEST--
MongoDB\BSON\ObjectID cannot be extended
--FILE--
<?php

class MyObjectID extends MongoDB\BSON\ObjectID {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyObjectID may not inherit from final class (MongoDB\BSON\ObjectID) in %s on line %d
