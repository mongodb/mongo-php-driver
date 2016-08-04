--TEST--
MongoDB\BSON\Regex cannot be extended
--FILE--
<?php

class MyRegex extends MongoDB\BSON\Regex {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyRegex may not inherit from final class (MongoDB\BSON\Regex) in %s on line %d
