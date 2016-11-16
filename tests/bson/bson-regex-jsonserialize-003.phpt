--TEST--
MongoDB\BSON\Regex::jsonSerialize() with json_encode() (without flags)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$doc = ['foo' => new MongoDB\BSON\Regex('pattern')];
$json = json_encode($doc);

echo toJSON(fromPHP($doc)), "\n";
echo $json, "\n";
var_dump(toPHP(fromJSON($json)));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
{ "foo" : { "$regex" : "pattern", "$options" : "" } }
{"foo":{"$regex":"pattern","$options":""}}
object(stdClass)#%d (%d) {
  ["foo"]=>
  object(MongoDB\BSON\Regex)#%d (%d) {
    ["pattern"]=>
    string(7) "pattern"
    ["flags"]=>
    string(0) ""
  }
}
===DONE===
