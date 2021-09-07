--TEST--
MongoDB\BSON\Javascript cannot be extended
--FILE--
<?php

class MyJavascript extends MongoDB\BSON\Javascript {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyJavascript %s final class %SMongoDB\BSON\Javascript%S in %s on line %d
