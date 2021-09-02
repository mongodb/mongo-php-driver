--TEST--
MongoDB\Driver\BulkWrite cannot be extended
--FILE--
<?php

class MyBulkWrite extends MongoDB\Driver\BulkWrite {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyBulkWrite %s final class %SMongoDB\Driver\BulkWrite%S in %s on line %d
