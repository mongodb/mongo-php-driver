--TEST--
BSON BSON\MinKey cannot be extended
--FILE--
<?php

use MongoDB\BSON as BSON;

class MyMinKey extends BSON\MinKey {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyMinKey may not inherit from final class (%SBSON\MinKey) in %s on line %d
