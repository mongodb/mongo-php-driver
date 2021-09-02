--TEST--
MongoDB\BSON\Regex cannot be extended
--FILE--
<?php

class MyRegex extends MongoDB\BSON\Regex {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyRegex %s final class %SMongoDB\BSON\Regex%S in %s on line %d
