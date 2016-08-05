--TEST--
MongoDB\BSON\Javascript cannot be extended
--FILE--
<?php

class MyJavascript extends MongoDB\BSON\Javascript {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyJavascript may not inherit from final class (MongoDB\BSON\Javascript) in %s on line %d
