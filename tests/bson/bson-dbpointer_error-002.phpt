--TEST--
MongoDB\BSON\DBPointer cannot be extended
--FILE--
<?php

class MyDBPointer extends MongoDB\BSON\DBPointer {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyDBPointer may not inherit from final class (MongoDB\BSON\DBPointer) in %s on line %d
