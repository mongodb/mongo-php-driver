--TEST--
MongoDB\BSON\Value::clone tests
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    'null' => null,
    'int32' => 2,
    // Int64 omitted to avoid failures on 32-bit platforms
    'float' => 3.14,
    'string' => 'foo',
    'true' => true,
    'false' => false,
    'list' => [1, 2, 3],
    'struct' => ['foo' => 'bar'],
    'object' => (object) ['foo' => 'bar'],
];

foreach ($tests as $name => $value) {
    printf("Testing %s:\n", $name);

    $bsonValue = MongoDB\BSON\Value::fromPHP($value);
    $cloned = clone $bsonValue;
    unset($bsonValue);
    var_dump($cloned);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing null:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(4) "null"
  ["value"]=>
  NULL
}
Testing int32:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(14) "32-bit integer"
  ["value"]=>
  int(2)
}
Testing float:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "double"
  ["value"]=>
  float(3.14)
}
Testing string:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "string"
  ["value"]=>
  string(3) "foo"
}
Testing true:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(7) "boolean"
  ["value"]=>
  bool(true)
}
Testing false:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(7) "boolean"
  ["value"]=>
  bool(false)
}
Testing list:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(5) "array"
  ["value"]=>
  array(3) {
    [0]=>
    int(1)
    [1]=>
    int(2)
    [2]=>
    int(3)
  }
}
Testing struct:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(8) "document"
  ["value"]=>
  object(stdClass)#%d (%d) {
    ["foo"]=>
    string(3) "bar"
  }
}
Testing object:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(8) "document"
  ["value"]=>
  object(stdClass)#%d (%d) {
    ["foo"]=>
    string(3) "bar"
  }
}
===DONE===
