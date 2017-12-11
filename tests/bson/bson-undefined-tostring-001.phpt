--TEST--
MongoDB\BSON\Undefined::__toString()
--FILE--
<?php

$undefined = MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "undefined": {"$undefined": true} }'))->undefined;
var_dump((string) $undefined);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(0) ""
===DONE===
