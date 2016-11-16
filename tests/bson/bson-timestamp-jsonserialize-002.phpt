--TEST--
MongoDB\BSON\Timestamp::jsonSerialize() with json_encode()
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$doc = ['foo' => new MongoDB\BSON\Timestamp('1234', '5678')];
$json = json_encode($doc);

echo toJSON(fromPHP($doc)), "\n";
echo $json, "\n";
var_dump(toPHP(fromJSON($json)));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
{ "foo" : { "$timestamp" : { "t" : 5678, "i" : 1234 } } }
{"foo":{"$timestamp":{"t":5678,"i":1234}}}
object(stdClass)#%d (%d) {
  ["foo"]=>
  object(MongoDB\BSON\Timestamp)#%d (%d) {
    ["increment"]=>
    string(4) "1234"
    ["timestamp"]=>
    string(4) "5678"
  }
}
===DONE===
