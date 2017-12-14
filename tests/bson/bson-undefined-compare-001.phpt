--TEST--
MongoDB\BSON\Undefined comparisons
--FILE--
<?php

var_dump(MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "undefined": {"$undefined": true} }')) == MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "undefined": {"$undefined": true} }')));
var_dump(MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "undefined": {"$undefined": true} }')) < MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "undefined": {"$undefined": true} }')));
var_dump(MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "undefined": {"$undefined": true} }')) > MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "undefined": {"$undefined": true} }')));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(false)
bool(false)
===DONE===
