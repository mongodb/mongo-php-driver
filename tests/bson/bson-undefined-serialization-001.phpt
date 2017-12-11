--TEST--
MongoDB\BSON\Undefined serialization
--FILE--
<?php

var_dump($undefined = MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "undefined": {"$undefined": true} }'))->undefined);
var_dump($s = serialize($undefined));
var_dump(unserialize($s));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Undefined)#%d (%d) {
}
string(34) "C:22:"MongoDB\BSON\Undefined":0:{}"
object(MongoDB\BSON\Undefined)#%d (%d) {
}
===DONE===
