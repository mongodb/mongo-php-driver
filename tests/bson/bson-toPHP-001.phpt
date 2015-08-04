--TEST--
BSON\toPHP(): __pclass must be both instantiatable and Persistable
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
use MongoDB\BSON as BSON;

require_once __DIR__ . "/../utils/basic.inc";

abstract class MyAbstractDocument implements BSON\Persistable
{
}

class MyDocument implements BSON\Unserializable
{
    public function bsonUnserialize(array $data)
    {
        $this->unserialized = true;
    }
}

// Create base64-encoded class names for __pclass field's binary data
$bMyAbstractDocument = base64_encode('MyAbstractDocument');
$bMyDocument = base64_encode('MyDocument');
$bUnserializable = base64_encode(BSON_NAMESPACE . '\Unserializable');
$bPersistable = base64_encode(BSON_NAMESPACE . '\Persistable');

$tests = array(
    '{ "foo": "yes", "__pclass": { "$type" : "80", "$binary" : "' . $bMyAbstractDocument . '" } }',
    '{ "foo": "yes", "__pclass": { "$type" : "80", "$binary" : "' . $bMyDocument . '" } }',
    '{ "foo": "yes", "__pclass": { "$type" : "80", "$binary" : "' . $bUnserializable . '" } }',
    '{ "foo": "yes", "__pclass": { "$type" : "44", "$binary" : "' . $bPersistable . '" } }',
);

foreach ($tests as $test) {
    echo $test, "\n";
    var_dump(toPHP(fromJSON($test)));
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
{ "foo": "yes", "__pclass": { "$type" : "80", "$binary" : "TXlBYnN0cmFjdERvY3VtZW50" } }
object(stdClass)#%d (2) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(MongoDB\BSON\Binary)#%d (2) {
    ["data"]=>
    string(18) "MyAbstractDocument"
    ["type"]=>
    int(128)
  }
}

{ "foo": "yes", "__pclass": { "$type" : "80", "$binary" : "TXlEb2N1bWVudA==" } }
object(stdClass)#%d (2) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(MongoDB\BSON\Binary)#%d (2) {
    ["data"]=>
    string(10) "MyDocument"
    ["type"]=>
    int(128)
  }
}

{ "foo": "yes", "__pclass": { "$type" : "80", "$binary" : "TW9uZ29EQlxCU09OXFVuc2VyaWFsaXphYmxl" } }
object(stdClass)#%d (2) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(MongoDB\BSON\Binary)#%d (2) {
    ["data"]=>
    string(27) "MongoDB\BSON\Unserializable"
    ["type"]=>
    int(128)
  }
}

{ "foo": "yes", "__pclass": { "$type" : "44", "$binary" : "TW9uZ29EQlxCU09OXFBlcnNpc3RhYmxl" } }
object(stdClass)#%d (2) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(MongoDB\BSON\Binary)#%d (2) {
    ["data"]=>
    string(24) "MongoDB\BSON\Persistable"
    ["type"]=>
    int(68)
  }
}

===DONE===
