--TEST--
MongoDB\Driver\WriteConcernError cannot be extended
--FILE--
<?php

class MyWriteConcernError extends MongoDB\Driver\WriteConcernError {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyWriteConcernError %s final class %SMongoDB\Driver\WriteConcernError%S in %s on line %d
