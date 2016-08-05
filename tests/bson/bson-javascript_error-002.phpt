--TEST--
BSON BSON\Javascript cannot be extended
--FILE--
<?php

use MongoDB\BSON as BSON;

class MyJavascript extends BSON\Javascript {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyJavascript may not inherit from final class (%SBSON\Javascript) in %s on line %d
