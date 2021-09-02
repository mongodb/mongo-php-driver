--TEST--
MongoDB\Driver\CursorId cannot be extended
--FILE--
<?php

class MyCursorId extends MongoDB\Driver\CursorId {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyCursorId %s final class %SMongoDB\Driver\CursorId%S in %s on line %d
