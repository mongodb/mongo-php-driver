--TEST--
MongoDB\Driver\WriteConcernError cannot be extended
--FILE--
<?php

class MyWriteConcernError extends MongoDB\Driver\WriteConcernError {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyWriteConcernError may not inherit from final class (MongoDB\Driver\WriteConcernError) in %s on line %d
