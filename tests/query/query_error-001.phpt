--TEST--
MongoDB\Driver\Query cannot be extended
--FILE--
<?php

class MyQuery extends MongoDB\Driver\Query {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyQuery may not inherit from final class (MongoDB\Driver\Query) in %s on line %d
