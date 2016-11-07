--TEST--
MongoDB\BSON\Timestamp::jsonSerialize() return value
--FILE--
<?php

$ts = new MongoDB\BSON\Timestamp('1234', '5678');
var_dump($ts->jsonSerialize());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(1) {
  ["$timestamp"]=>
  array(2) {
    ["t"]=>
    int(5678)
    ["i"]=>
    int(1234)
  }
}
===DONE===
