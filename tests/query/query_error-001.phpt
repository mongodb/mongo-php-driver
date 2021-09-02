--TEST--
MongoDB\Driver\Query cannot be extended
--FILE--
<?php

class MyQuery extends MongoDB\Driver\Query {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyQuery %s final class %SMongoDB\Driver\Query%S in %s on line %d
