--TEST--
PHPC-2456: References passed in a typeMap
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$fieldPaths = [];

$typeMap = ['fieldPaths' => &$fieldPaths];

var_dump(MongoDB\BSON\Document::fromPHP([])->toPHP($typeMap));
var_dump(MongoDB\BSON\PackedArray::fromPHP([])->toPHP($typeMap));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (0) {
}
array(0) {
}
===DONE===
