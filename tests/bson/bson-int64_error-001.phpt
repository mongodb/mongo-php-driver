--TEST--
MongoDB\BSON\Int64 cannot be extended
--FILE--
<?php

class MyInt64 extends MongoDB\BSON\Int64 {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyInt64 may not inherit from final class (MongoDB\BSON\Int64) in %s on line %d
