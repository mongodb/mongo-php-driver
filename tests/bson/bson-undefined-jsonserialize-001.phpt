--TEST--
MongoDB\BSON\Undefined::jsonSerialize() return value
--FILE--
<?php

$undefined = MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "undefined": {"$undefined": true} }'))->undefined;
var_dump($undefined->jsonSerialize());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(1) {
  ["$undefined"]=>
  bool(true)
}
===DONE===
