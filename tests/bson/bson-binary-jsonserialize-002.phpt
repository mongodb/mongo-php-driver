--TEST--
MongoDB\BSON\Binary::jsonSerialize() with json_encode()
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$doc = ['foo' => new MongoDB\BSON\Binary('gargleblaster', 24)];
$json = json_encode($doc);

echo toJSON(fromPHP($doc)), "\n";
echo $json, "\n";
var_dump(toPHP(fromJSON($json)));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
{ "foo" : { "$binary" : "Z2FyZ2xlYmxhc3Rlcg==", "$type" : "18" } }
{"foo":{"$binary":"Z2FyZ2xlYmxhc3Rlcg==","$type":"18"}}
object(stdClass)#%d (%d) {
  ["foo"]=>
  object(MongoDB\BSON\Binary)#%d (%d) {
    ["data"]=>
    string(13) "gargleblaster"
    ["type"]=>
    int(24)
  }
}
===DONE===
