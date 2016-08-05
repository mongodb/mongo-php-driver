--TEST--
BSON BSON\MaxKey cannot be extended
--FILE--
<?php

use MongoDB\BSON as BSON;

class MyMaxKey extends BSON\MaxKey {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyMaxKey may not inherit from final class (%SBSON\MaxKey) in %s on line %d
