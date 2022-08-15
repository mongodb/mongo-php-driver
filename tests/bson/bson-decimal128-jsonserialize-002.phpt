--TEST--
MongoDB\BSON\Decimal128::jsonSerialize() with json_encode()
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$doc = ['foo' => new MongoDB\BSON\Decimal128('12389719287312')];
$json = json_encode($doc);

echo toJSON(fromPHP($doc)), "\n";
echo $json, "\n";
var_dump(toPHP(fromJSON($json)));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
{ "foo" : { "$numberDecimal" : "12389719287312" } }
{"foo":{"$numberDecimal":"12389719287312"}}
object(stdClass)#%d (%d) {
  ["foo"]=>
  object(MongoDB\BSON\Decimal128)#%d (%d) {
    ["dec"]=>
    string(14) "12389719287312"
  }
}
===DONE===
