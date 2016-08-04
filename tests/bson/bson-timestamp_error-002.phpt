--TEST--
BSON BSON\Timestamp cannot be extended
--FILE--
<?php

use MongoDB\BSON as BSON;

class MyTimestamp extends BSON\Timestamp {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyTimestamp may not inherit from final class (%SBSON\Timestamp) in %s on line %d
