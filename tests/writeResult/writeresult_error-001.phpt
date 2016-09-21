--TEST--
MongoDB\Driver\WriteResult cannot be extended
--FILE--
<?php

class MyWriteResult extends MongoDB\Driver\WriteResult {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyWriteResult may not inherit from final class (MongoDB\Driver\WriteResult) in %s on line %d
