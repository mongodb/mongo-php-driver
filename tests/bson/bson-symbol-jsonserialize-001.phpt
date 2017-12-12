--TEST--
MongoDB\BSON\Symbol::jsonSerialize() return value
--FILE--
<?php

$js = MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "symbol": {"$symbol": "valSymbol"} }'))->symbol;
var_dump($js->jsonSerialize());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(1) {
  ["$symbol"]=>
  string(9) "valSymbol"
}
===DONE===
