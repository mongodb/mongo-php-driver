--TEST--
MongoDB\BSON\Timestamp cannot be extended
--FILE--
<?php

class MyTimestamp extends MongoDB\BSON\Timestamp {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyTimestamp %s final class %SMongoDB\BSON\Timestamp%S in %s on line %d
