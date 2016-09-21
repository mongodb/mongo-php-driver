--TEST--
MongoDB\Driver\Cursor cannot be extended
--FILE--
<?php

class MyCursor extends MongoDB\Driver\Cursor {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyCursor may not inherit from final class (MongoDB\Driver\Cursor) in %s on line %d
