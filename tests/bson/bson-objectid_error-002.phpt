--TEST--
MongoDB\BSON\ObjectId cannot be extended
--FILE--
<?php

class MyObjectId extends MongoDB\BSON\ObjectId {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyObjectId %s final class %SMongoDB\BSON\ObjectId%S in %s on line %d
