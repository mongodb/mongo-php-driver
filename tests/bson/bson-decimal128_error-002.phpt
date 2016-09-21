--TEST--
MongoDB\BSON\Decimal128 cannot be extended
--FILE--
<?php

class MyDecimal128 extends MongoDB\BSON\Decimal128 {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyDecimal128 may not inherit from final class (MongoDB\BSON\Decimal128) in %s on line %d
