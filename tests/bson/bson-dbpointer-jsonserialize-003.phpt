--TEST--
MongoDB\BSON\DBPointer::jsonSerialize() with json_encode()
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$doc = MongoDB\BSON\Document::fromJSON('{ "foo": {"$dbPointer": {"$ref": "phongo.test", "$id" : { "$oid" : "5a2e78accd485d55b4050000" }  }} }')->toPHP();
$json = json_encode($doc);

echo toJSON(fromPHP($doc)), "\n";
echo $json, "\n";
var_dump(toPHP(fromJSON($json)));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
{ "foo" : { "$dbPointer" : { "$ref" : "phongo.test", "$id" : { "$oid" : "5a2e78accd485d55b4050000" } } } }
{"foo":{"$dbPointer":{"$ref":"phongo.test","$id":{"$oid":"5a2e78accd485d55b4050000"}}}}
object(stdClass)#%d (%d) {
  ["foo"]=>
  object(MongoDB\BSON\DBPointer)#%d (%d) {
    ["ref"]=>
    string(11) "phongo.test"
    ["id"]=>
    string(24) "5a2e78accd485d55b4050000"
  }
}
===DONE===
