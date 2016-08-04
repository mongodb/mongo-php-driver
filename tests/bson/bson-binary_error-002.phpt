--TEST--
MongoDB\BSON\Binary cannot be extended
--FILE--
<?php

class MyBinary extends MongoDB\BSON\Binary {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyBinary may not inherit from final class (MongoDB\BSON\Binary) in %s on line %d
