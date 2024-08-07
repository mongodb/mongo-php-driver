--TEST--
MongoDB\BSON\Undefined::__set_state()
--FILE--
<?php

$undefined = MongoDB\BSON\Document::fromJSON('{ "undefined": {"$undefined": true} }')->toPHP()->undefined;

$s = var_export($undefined, true);
echo $s, "\n";

var_dump(eval('return ' . $s . ';'));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%r\\?%rMongoDB\BSON\Undefined::__set_state(array(
))
object(MongoDB\BSON\Undefined)#%d (%d) {
}
===DONE===
