--TEST--
MongoDB\BSON\UTCDateTime cannot be extended
--FILE--
<?php

class MyUTCDateTime extends MongoDB\BSON\UTCDateTime {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyUTCDateTime %s final class %SMongoDB\BSON\UTCDateTime%S in %s on line %d
