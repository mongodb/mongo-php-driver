--TEST--
MongoDB\BSON\Undefined::__toString()
--FILE--
<?php

$undefined = MongoDB\BSON\Document::fromJSON('{ "undefined": {"$undefined": true} }')->toPHP()->undefined;
var_dump((string) $undefined);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(0) ""
===DONE===
