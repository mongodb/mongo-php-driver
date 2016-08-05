--TEST--
MongoDB\BSON\MaxKey cannot be extended
--FILE--
<?php

class MyMaxKey extends MongoDB\BSON\MaxKey {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyMaxKey may not inherit from final class (MongoDB\BSON\MaxKey) in %s on line %d
