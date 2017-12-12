--TEST--
MongoDB\BSON\Symbol cannot be extended
--FILE--
<?php

class MySymbol extends MongoDB\BSON\Symbol {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MySymbol may not inherit from final class (MongoDB\BSON\Symbol) in %s on line %d
