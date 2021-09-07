--TEST--
MongoDB\BSON\MaxKey cannot be extended
--FILE--
<?php

class MyMaxKey extends MongoDB\BSON\MaxKey {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyMaxKey %s final class %SMongoDB\BSON\MaxKey%S in %s on line %d
