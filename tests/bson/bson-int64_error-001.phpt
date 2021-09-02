--TEST--
MongoDB\BSON\Int64 cannot be extended
--FILE--
<?php

class MyInt64 extends MongoDB\BSON\Int64 {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyInt64 %s final class %SMongoDB\BSON\Int64%S in %s on line %d
