--TEST--
BSON BSON\UTCDateTime cannot be extended
--FILE--
<?php

use MongoDB\BSON as BSON;

class MyUTCDateTime extends BSON\UTCDateTime {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyUTCDateTime may not inherit from final class (%SBSON\UTCDateTime) in %s on line %d
