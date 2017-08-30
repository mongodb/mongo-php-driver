--TEST--
MongoDB\BSON\ObjectId cannot be extended
--FILE--
<?php

class MyObjectId extends MongoDB\BSON\ObjectId {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyObjectId may not inherit from final class (MongoDB\BSON\ObjectId) in %s on line %d
