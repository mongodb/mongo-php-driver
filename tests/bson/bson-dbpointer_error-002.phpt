--TEST--
MongoDB\BSON\DBPointer cannot be extended
--FILE--
<?php

class MyDBPointer extends MongoDB\BSON\DBPointer {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyDBPointer %s final class %SMongoDB\BSON\DBPointer%S in %s on line %d
