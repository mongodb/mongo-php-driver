--TEST--
BSON encoding: Encoding objects into BSON representation
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MyClass implements BSON\Persistable {
    function bsonSerialize() {
        return array(
            "random" => "class",
            "data"
        );
    }
    function bsonUnserialize(array $data) {
        $this->props = $data;
    }
}
class MyClass2 implements BSON\Persistable {
    function bsonSerialize() {
        return array(
            1, 2, 3,
        );
    }
    function bsonUnserialize(array $data) {
        $this->props = $data;
    }
}

$tests = array(
    array("stuff" => new MyClass),
    array("stuff" => new MyClass2),
    array("stuff" => array(new MyClass, new MyClass2)),
);

foreach($tests as $n => $test) {
    $s = BSON\fromArray($test);
    echo "Test#{$n} ", BSON\toJSON($s), "\n";
    hex_dump($s);
    $ret = BSON\toArray($s);
    var_dump($ret);
}
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Test#0 { "stuff" : [ { "$type" : "80", "$binary" : "TXlDbGFzcw==" }, "class", "data" ] }
     0 : 3f 00 00 00 04 73 74 75 66 66 00 33 00 00 00 05  [?....stuff.3....]
    10 : 5f 5f 00 07 00 00 00 80 4d 79 43 6c 61 73 73 02  [__......MyClass.]
    20 : 72 61 6e 64 6f 6d 00 06 00 00 00 63 6c 61 73 73  [random.....class]
    30 : 00 02 30 00 05 00 00 00 64 61 74 61 00 00 00     [..0.....data...]
array(1) {
  ["stuff"]=>
  object(MyClass)#6 (1) {
    ["props"]=>
    array(2) {
      ["random"]=>
      string(5) "class"
      [0]=>
      string(4) "data"
    }
  }
}
Test#1 { "stuff" : [ { "$type" : "80", "$binary" : "TXlDbGFzczI=" }, 1, 2, 3 ] }
     0 : 37 00 00 00 04 73 74 75 66 66 00 2b 00 00 00 05  [7....stuff.+....]
    10 : 5f 5f 00 08 00 00 00 80 4d 79 43 6c 61 73 73 32  [__......MyClass2]
    20 : 10 30 00 01 00 00 00 10 31 00 02 00 00 00 10 32  [.0......1......2]
    30 : 00 03 00 00 00 00 00                             [.......]
array(1) {
  ["stuff"]=>
  object(MyClass2)#7 (1) {
    ["props"]=>
    array(3) {
      [0]=>
      int(1)
      [1]=>
      int(2)
      [2]=>
      int(3)
    }
  }
}
Test#2 { "stuff" : [ [ { "$type" : "80", "$binary" : "TXlDbGFzcw==" }, "class", "data" ], [ { "$type" : "80", "$binary" : "TXlDbGFzczI=" }, 1, 2, 3 ] ] }
     0 : 75 00 00 00 04 73 74 75 66 66 00 69 00 00 00 04  [u....stuff.i....]
    10 : 30 00 33 00 00 00 05 5f 5f 00 07 00 00 00 80 4d  [0.3....__......M]
    20 : 79 43 6c 61 73 73 02 72 61 6e 64 6f 6d 00 06 00  [yClass.random...]
    30 : 00 00 63 6c 61 73 73 00 02 30 00 05 00 00 00 64  [..class..0.....d]
    40 : 61 74 61 00 00 04 31 00 2b 00 00 00 05 5f 5f 00  [ata...1.+....__.]
    50 : 08 00 00 00 80 4d 79 43 6c 61 73 73 32 10 30 00  [.....MyClass2.0.]
    60 : 01 00 00 00 10 31 00 02 00 00 00 10 32 00 03 00  [.....1......2...]
    70 : 00 00 00 00 00                                   [.....]
array(1) {
  ["stuff"]=>
  array(2) {
    [0]=>
    object(MyClass)#6 (1) {
      ["props"]=>
      array(2) {
        ["random"]=>
        string(5) "class"
        [0]=>
        string(4) "data"
      }
    }
    [1]=>
    object(MyClass2)#8 (1) {
      ["props"]=>
      array(3) {
        [0]=>
        int(1)
        [1]=>
        int(2)
        [2]=>
        int(3)
      }
    }
  }
}
===DONE===
