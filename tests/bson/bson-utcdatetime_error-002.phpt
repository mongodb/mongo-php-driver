--TEST--
MongoDB\BSON\UTCDateTime cannot be extended
--FILE--
<?php

class MyUTCDateTime extends MongoDB\BSON\UTCDateTime {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyUTCDateTime may not inherit from final class (MongoDB\BSON\UTCDateTime) in %s on line %d
