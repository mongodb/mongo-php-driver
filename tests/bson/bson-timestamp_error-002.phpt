--TEST--
MongoDB\BSON\Timestamp cannot be extended
--FILE--
<?php

class MyTimestamp extends MongoDB\BSON\Timestamp {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyTimestamp may not inherit from final class (MongoDB\BSON\Timestamp) in %s on line %d
