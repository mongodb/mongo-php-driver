--TEST--
MongoDB\BSON\Undefined comparisons
--FILE--
<?php

var_dump(MongoDB\BSON\Document::fromJSON('{ "undefined": {"$undefined": true} }')->toPHP() == MongoDB\BSON\Document::fromJSON('{ "undefined": {"$undefined": true} }')->toPHP());
var_dump(MongoDB\BSON\Document::fromJSON('{ "undefined": {"$undefined": true} }')->toPHP() < MongoDB\BSON\Document::fromJSON('{ "undefined": {"$undefined": true} }')->toPHP());
var_dump(MongoDB\BSON\Document::fromJSON('{ "undefined": {"$undefined": true} }')->toPHP() > MongoDB\BSON\Document::fromJSON('{ "undefined": {"$undefined": true} }')->toPHP());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(false)
bool(false)
===DONE===
