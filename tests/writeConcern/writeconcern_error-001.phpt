--TEST--
MongoDB\Driver\WriteConcern cannot be extended
--FILE--
<?php

class MyWriteConcern extends MongoDB\Driver\WriteConcern {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyWriteConcern %s final class %SMongoDB\Driver\WriteConcern%S in %s on line %d
