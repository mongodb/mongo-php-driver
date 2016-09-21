--TEST--
MongoDB\Driver\BulkWrite cannot be extended
--FILE--
<?php

class MyBulkWrite extends MongoDB\Driver\BulkWrite {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyBulkWrite may not inherit from final class (MongoDB\Driver\BulkWrite) in %s on line %d
