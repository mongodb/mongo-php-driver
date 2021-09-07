--TEST--
MongoDB\Driver\ReadConcern cannot be extended
--FILE--
<?php

class MyReadConcern extends MongoDB\Driver\ReadConcern {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyReadConcern %s final class %SMongoDB\Driver\ReadConcern%S in %s on line %d
