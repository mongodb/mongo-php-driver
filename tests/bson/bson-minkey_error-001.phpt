--TEST--
MongoDB\BSON\MinKey cannot be extended
--FILE--
<?php

class MyMinKey extends MongoDB\BSON\MinKey {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyMinKey %s final class %SMongoDB\BSON\MinKey%S in %s on line %d
