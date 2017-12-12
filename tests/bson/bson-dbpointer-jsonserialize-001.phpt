--TEST--
MongoDB\BSON\DBPointer::jsonSerialize() return value
--FILE--
<?php

$dbref = MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "dbref": {"$dbPointer": {"$ref": "phongo.test", "$id" : { "$oid" : "5a2e78accd485d55b4050000" }  }} }'))->dbref;
var_dump($dbref->jsonSerialize());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(1) {
  ["$dbPointer"]=>
  array(2) {
    ["$ref"]=>
    string(11) "phongo.test"
    ["$id"]=>
    array(1) {
      ["$oid"]=>
      string(24) "5a2e78accd485d55b4050000"
    }
  }
}
===DONE===
