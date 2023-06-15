--TEST--
MongoDB\BSON\DBPointer::__set_state()
--FILE--
<?php

$dbPointer = MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "dbref": {"$dbPointer": {"$ref": "phongo.test", "$id" : { "$oid" : "5a2e78accd485d55b4050000" }  }} }'))->dbref;

$s = var_export($dbPointer, true);
echo $s, "\n";

var_dump(eval('return ' . $s . ';'));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%r\\?%rMongoDB\BSON\DBPointer::__set_state(array(
%w'ref' => 'phongo.test',
%w'id' => '5a2e78accd485d55b4050000',
))
object(MongoDB\BSON\DBPointer)#%d (%d) {
  ["ref"]=>
  string(11) "phongo.test"
  ["id"]=>
  string(24) "5a2e78accd485d55b4050000"
}
===DONE===
