--TEST--
MongoDB\Driver\WriteResult cannot be extended
--FILE--
<?php

class MyWriteResult extends MongoDB\Driver\WriteResult {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyWriteResult %s final class %SMongoDB\Driver\WriteResult%S in %s on line %d
