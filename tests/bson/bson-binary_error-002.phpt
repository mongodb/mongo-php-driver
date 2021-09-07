--TEST--
MongoDB\BSON\Binary cannot be extended
--FILE--
<?php

class MyBinary extends MongoDB\BSON\Binary {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyBinary %s final class %SMongoDB\BSON\Binary%S in %s on line %d
