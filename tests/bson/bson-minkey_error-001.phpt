--TEST--
MongoDB\BSON\MinKey cannot be extended
--FILE--
<?php

class MyMinKey extends MongoDB\BSON\MinKey {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyMinKey may not inherit from final class (MongoDB\BSON\MinKey) in %s on line %d
