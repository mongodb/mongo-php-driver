--TEST--
MongoDB\Driver\ReadConcern cannot be extended
--FILE--
<?php

class MyReadConcern extends MongoDB\Driver\ReadConcern {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyReadConcern may not inherit from final class (MongoDB\Driver\ReadConcern) in %s on line %d
