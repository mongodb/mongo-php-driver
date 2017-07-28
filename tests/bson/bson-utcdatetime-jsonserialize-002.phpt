--TEST--
MongoDB\BSON\UTCDateTime::jsonSerialize() with json_encode()
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$doc = ['foo' => new MongoDB\BSON\UTCDateTime(new DateTime('2016-10-11 13:34:26.817 UTC'))];
$json = json_encode($doc);

echo toJSON(fromPHP($doc)), "\n";
echo $json, "\n";
var_dump(toPHP(fromJSON($json)));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
{ "foo" : { "$date" : 1476192866817 } }
{"foo":{"$date":{"$numberLong":"1476192866817"}}}
object(stdClass)#%d (%d) {
  ["foo"]=>
  object(MongoDB\BSON\UTCDateTime)#%d (%d) {
    ["milliseconds"]=>
    string(13) "1476192866817"
  }
}
===DONE===
