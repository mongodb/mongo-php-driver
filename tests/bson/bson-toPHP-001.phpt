--TEST--
MongoDB\BSON\toPHP(): __pclass must be both instantiatable and Persistable
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

abstract class MyAbstractDocument implements MongoDB\BSON\Persistable
{
}

class MyDocument implements MongoDB\BSON\Unserializable
{
    public function bsonUnserialize(array $data)
    {
        $this->unserialized = true;
    }
}

// Create base64-encoded class names for __pclass field's binary data
$bMyAbstractDocument = base64_encode('MyAbstractDocument');
$bMyDocument = base64_encode('MyDocument');
$bUnserializable = base64_encode('MongoDB\BSON\Unserializable');
$bPersistable = base64_encode('MongoDB\BSON\Persistable');

$tests = array(
    '{ "foo": "yes", "__pclass": { "$binary": "' . $bMyAbstractDocument . '", "$type": "80" } }',
    '{ "foo": "yes", "__pclass": { "$binary": "' . $bMyDocument . '", "$type": "80" } }',
    '{ "foo": "yes", "__pclass": { "$binary": "' . $bUnserializable . '", "$type": "80" } }',
    '{ "foo": "yes", "__pclass": { "$binary": "' . $bPersistable . '", "$type": "44" } }',
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
{ "foo": "yes", "__pclass": { "$binary": "TXlBYnN0cmFjdERvY3VtZW50", "$type": "80" } }
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

{ "foo": "yes", "__pclass": { "$binary": "TXlEb2N1bWVudA==", "$type": "80" } }
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

{ "foo": "yes", "__pclass": { "$binary": "TW9uZ29EQlxCU09OXFVuc2VyaWFsaXphYmxl", "$type": "80" } }
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

{ "foo": "yes", "__pclass": { "$binary": "TW9uZ29EQlxCU09OXFBlcnNpc3RhYmxl", "$type": "44" } }
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
