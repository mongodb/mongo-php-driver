--TEST--
MongoDB\Driver\CursorID cannot be extended
--FILE--
<?php

class MyCursorID extends MongoDB\Driver\CursorID {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyCursorID may not inherit from final class (MongoDB\Driver\CursorID) in %s on line %d
