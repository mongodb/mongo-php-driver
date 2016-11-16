--TEST--
MongoDB\BSON\MaxKey::jsonSerialize() with json_encode()
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$doc = ['foo' => new MongoDB\BSON\MaxKey];
$json = json_encode($doc);

echo toJSON(fromPHP($doc)), "\n";
echo $json, "\n";
var_dump(toPHP(fromJSON($json)));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
{ "foo" : { "$maxKey" : 1 } }
{"foo":{"$maxKey":1}}
object(stdClass)#%d (%d) {
  ["foo"]=>
  object(MongoDB\BSON\MaxKey)#%d (%d) {
  }
}
===DONE===
