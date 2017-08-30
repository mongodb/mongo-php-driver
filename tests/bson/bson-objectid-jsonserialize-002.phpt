--TEST--
MongoDB\BSON\ObjectId::jsonSerialize() with json_encode()
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$doc = ['foo' => new MongoDB\BSON\ObjectId('5820ca4bef62d52d9924d0d8')];
$json = json_encode($doc);

echo toJSON(fromPHP($doc)), "\n";
echo $json, "\n";
var_dump(toPHP(fromJSON($json)));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
{ "foo" : { "$oid" : "5820ca4bef62d52d9924d0d8" } }
{"foo":{"$oid":"5820ca4bef62d52d9924d0d8"}}
object(stdClass)#%d (%d) {
  ["foo"]=>
  object(MongoDB\BSON\ObjectId)#%d (%d) {
    ["oid"]=>
    string(24) "5820ca4bef62d52d9924d0d8"
  }
}
===DONE===
