--TEST--
MongoDB\BSON\Timestamp debug handler
--FILE--
<?php

$timestamp = new MongoDB\BSON\Timestamp(1234, 5678);

var_dump($timestamp);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  string(4) "1234"
  ["timestamp"]=>
  string(4) "5678"
}
===DONE===
