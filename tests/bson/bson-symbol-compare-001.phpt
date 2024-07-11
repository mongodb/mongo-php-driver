--TEST--
MongoDB\BSON\Symbol comparisons
--FILE--
<?php

var_dump(MongoDB\BSON\Document::fromJSON('{ "symbol": {"$symbol": "val1"} }')->toPHP() == MongoDB\BSON\Document::fromJSON('{ "symbol": {"$symbol": "val1"} }')->toPHP());
var_dump(MongoDB\BSON\Document::fromJSON('{ "symbol": {"$symbol": "val1"} }')->toPHP() < MongoDB\BSON\Document::fromJSON('{ "symbol": {"$symbol": "val2"} }')->toPHP());
var_dump(MongoDB\BSON\Document::fromJSON('{ "symbol": {"$symbol": "val1"} }')->toPHP() > MongoDB\BSON\Document::fromJSON('{ "symbol": {"$symbol": "val0"} }')->toPHP());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
bool(true)
===DONE===
