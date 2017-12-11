--TEST--
MongoDB\BSON\Undefined::jsonSerialize() with json_encode()
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$doc = MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "foo": {"$undefined": true} }'));
$json = json_encode($doc);

echo toJSON(fromPHP($doc)), "\n";
echo $json, "\n";
var_dump(toPHP(fromJSON($json)));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
{ "foo" : { "$undefined" : true } }
{"foo":{"$undefined":true}}
object(stdClass)#%d (%d) {
  ["foo"]=>
  object(MongoDB\BSON\Undefined)#%d (%d) {
  }
}
===DONE===
