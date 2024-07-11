--TEST--
MongoDB\BSON\Symbol::jsonSerialize() with json_encode()
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$doc = MongoDB\BSON\Document::fromJSON('{ "foo": {"$symbol": "symbolValue"} }')->toPHP();
$json = json_encode($doc);

echo toJSON(fromPHP($doc)), "\n";
echo $json, "\n";
var_dump(toPHP(fromJSON($json)));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
{ "foo" : { "$symbol" : "symbolValue" } }
{"foo":{"$symbol":"symbolValue"}}
object(stdClass)#%d (%d) {
  ["foo"]=>
  object(MongoDB\BSON\Symbol)#%d (%d) {
    ["symbol"]=>
    string(11) "symbolValue"
  }
}
===DONE===
