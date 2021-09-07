--TEST--
MongoDB\Driver\Cursor cannot be extended
--FILE--
<?php

class MyCursor extends MongoDB\Driver\Cursor {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyCursor %s final class %SMongoDB\Driver\Cursor%S in %s on line %d
