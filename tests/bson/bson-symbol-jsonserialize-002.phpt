--TEST--
MongoDB\BSON\Symbol::jsonSerialize() with json_encode()
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$doc = MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "foo": {"$symbol": "symbolValue"} }'));
$json = json_encode($doc);

echo toJSON(fromPHP($doc)), "\n";
echo $json, "\n";
var_dump(toPHP(fromJSON($json)));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
{ "foo" : "symbolValue" }
{"foo":{"$symbol":"symbolValue"}}
object(stdClass)#%d (%d) {
  ["foo"]=>
  object(MongoDB\BSON\Symbol)#%d (%d) {
    ["symbol"]=>
    string(11) "symbolValue"
  }
}
===DONE===
