--TEST--
MongoDB\BSON\MaxKey serialization
--FILE--
<?php

var_dump($maxkey = new MongoDB\BSON\MaxKey);
var_dump($s = serialize($maxkey));
var_dump(unserialize($s));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\MaxKey)#%d (%d) {
}
string(31) "C:19:"MongoDB\BSON\MaxKey":0:{}"
object(MongoDB\BSON\MaxKey)#%d (%d) {
}
===DONE===
