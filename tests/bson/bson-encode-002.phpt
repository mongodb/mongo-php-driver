--TEST--
BSON encoding: Encoding objects into BSON representation
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MyClass implements BSON\Serializable, BSON\Unserializable {
    function bsonSerialize() {
        return array(
            "random" => "class",
            "data"
        );
    }
    function bsonUnserialize(array $data) {
        var_dump(__METHOD__, $data);
    }
}
class MyClass2 implements BSON\Serializable, BSON\Unserializable {
    function bsonSerialize() {
        return array(
            1, 2, 3,
        );
    }
    function bsonUnserialize(array $data) {
        var_dump(__METHOD__, $data);
    }
}

$tests = array(
    array("stuff" => new MyClass),
    array("stuff" => new MyClass2),
);

foreach($tests as $n => $test) {
    $s = BSON\fromArray($test);
    echo "Test#{$n} ", BSON\toJSON($s), "\n";
    hex_dump($s);
    $obj = BSON\toArray($s, array("document" => get_class($test["stuff"])));
    var_dump($obj);
}
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Test#0 { "stuff" : { "random" : "class", "0" : "data" } }
     0 : 2f 00 00 00 03 73 74 75 66 66 00 23 00 00 00 02  [/....stuff.#....]
    10 : 72 61 6e 64 6f 6d 00 06 00 00 00 63 6c 61 73 73  [random.....class]
    20 : 00 02 30 00 05 00 00 00 64 61 74 61 00 00 00     [..0.....data...]
string(24) "MyClass::bsonUnserialize"
array(2) {
  ["random"]=>
  string(5) "class"
  [0]=>
  string(4) "data"
}
array(1) {
  ["stuff"]=>
  object(MyClass)#%d (0) {
  }
}
Test#1 { "stuff" : { "0" : 1, "1" : 2, "2" : 3 } }
     0 : 26 00 00 00 03 73 74 75 66 66 00 1a 00 00 00 10  [&....stuff......]
    10 : 30 00 01 00 00 00 10 31 00 02 00 00 00 10 32 00  [0......1......2.]
    20 : 03 00 00 00 00 00                                [......]
string(25) "MyClass2::bsonUnserialize"
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
array(1) {
  ["stuff"]=>
  object(MyClass2)#%d (0) {
  }
}
===DONE===
