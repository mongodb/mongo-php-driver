--TEST--
MongoDB\Driver\CursorId cannot be extended
--FILE--
<?php

class MyCursorId extends MongoDB\Driver\CursorId {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyCursorId may not inherit from final class (MongoDB\Driver\CursorId) in %s on line %d
