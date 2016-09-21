--TEST--
MongoDB\Driver\WriteConcern cannot be extended
--FILE--
<?php

class MyWriteConcern extends MongoDB\Driver\WriteConcern {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyWriteConcern may not inherit from final class (MongoDB\Driver\WriteConcern) in %s on line %d
