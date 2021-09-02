--TEST--
MongoDB\BSON\Symbol cannot be extended
--FILE--
<?php

class MySymbol extends MongoDB\BSON\Symbol {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MySymbol %s final class %SMongoDB\BSON\Symbol%S in %s on line %d
