--TEST--
BSON BSON\ObjectID cannot be extended
--FILE--
<?php

use MongoDB\BSON as BSON;

class MyObjectID extends BSON\ObjectID {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyObjectID may not inherit from final class (%SBSON\ObjectID) in %s on line %d
