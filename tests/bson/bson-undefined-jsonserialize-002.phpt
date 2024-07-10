--TEST--
MongoDB\BSON\Undefined::jsonSerialize() with json_encode()
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$doc = MongoDB\BSON\Document::fromJSON('{ "foo": {"$undefined": true} }')->toPHP();
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
