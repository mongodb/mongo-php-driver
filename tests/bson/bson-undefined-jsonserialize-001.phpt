--TEST--
MongoDB\BSON\Undefined::jsonSerialize() return value
--FILE--
<?php

$undefined = MongoDB\BSON\Document::fromJSON('{ "undefined": {"$undefined": true} }')->toPHP()->undefined;
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
