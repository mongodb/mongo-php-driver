--TEST--
BSON encoding: Encoding objects into BSON representation
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class MyClass implements MongoDB\BSON\Persistable {
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
class MyClass2 implements MongoDB\BSON\Persistable {
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
    $s = fromPHP($test);
    echo "Test#{$n} ", toJSON($s), "\n";
    hex_dump($s);
    $ret = toPHP($s);
    var_dump($ret);
}
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Test#0 { "stuff" : { "__pclass" : { "$binary" : "TXlDbGFzcw==", "$type" : "80" }, "random" : "class", "0" : "data" } }
     0 : 45 00 00 00 03 73 74 75 66 66 00 39 00 00 00 05  [E....stuff.9....]
    10 : 5f 5f 70 63 6c 61 73 73 00 07 00 00 00 80 4d 79  [__pclass......My]
    20 : 43 6c 61 73 73 02 72 61 6e 64 6f 6d 00 06 00 00  [Class.random....]
    30 : 00 63 6c 61 73 73 00 02 30 00 05 00 00 00 64 61  [.class..0.....da]
    40 : 74 61 00 00 00                                   [ta...]
object(stdClass)#%d (1) {
  ["stuff"]=>
  object(MyClass)#%d (1) {
    ["props"]=>
    array(3) {
      ["__pclass"]=>
      object(MongoDB\BSON\Binary)#%d (2) {
        ["data"]=>
        string(7) "MyClass"
        ["type"]=>
        int(128)
      }
      ["random"]=>
      string(5) "class"
      [0]=>
      string(4) "data"
    }
  }
}
Test#1 { "stuff" : { "__pclass" : { "$binary" : "TXlDbGFzczI=", "$type" : "80" }, "0" : 1, "1" : 2, "2" : 3 } }
     0 : 3d 00 00 00 03 73 74 75 66 66 00 31 00 00 00 05  [=....stuff.1....]
    10 : 5f 5f 70 63 6c 61 73 73 00 08 00 00 00 80 4d 79  [__pclass......My]
    20 : 43 6c 61 73 73 32 10 30 00 01 00 00 00 10 31 00  [Class2.0......1.]
    30 : 02 00 00 00 10 32 00 03 00 00 00 00 00           [.....2.......]
object(stdClass)#%d (1) {
  ["stuff"]=>
  object(MyClass2)#%d (1) {
    ["props"]=>
    array(4) {
      ["__pclass"]=>
      object(MongoDB\BSON\Binary)#%d (2) {
        ["data"]=>
        string(8) "MyClass2"
        ["type"]=>
        int(128)
      }
      [0]=>
      int(1)
      [1]=>
      int(2)
      [2]=>
      int(3)
    }
  }
}
Test#2 { "stuff" : [ { "__pclass" : { "$binary" : "TXlDbGFzcw==", "$type" : "80" }, "random" : "class", "0" : "data" }, { "__pclass" : { "$binary" : "TXlDbGFzczI=", "$type" : "80" }, "0" : 1, "1" : 2, "2" : 3 } ] }
     0 : 81 00 00 00 04 73 74 75 66 66 00 75 00 00 00 03  [.....stuff.u....]
    10 : 30 00 39 00 00 00 05 5f 5f 70 63 6c 61 73 73 00  [0.9....__pclass.]
    20 : 07 00 00 00 80 4d 79 43 6c 61 73 73 02 72 61 6e  [.....MyClass.ran]
    30 : 64 6f 6d 00 06 00 00 00 63 6c 61 73 73 00 02 30  [dom.....class..0]
    40 : 00 05 00 00 00 64 61 74 61 00 00 03 31 00 31 00  [.....data...1.1.]
    50 : 00 00 05 5f 5f 70 63 6c 61 73 73 00 08 00 00 00  [...__pclass.....]
    60 : 80 4d 79 43 6c 61 73 73 32 10 30 00 01 00 00 00  [.MyClass2.0.....]
    70 : 10 31 00 02 00 00 00 10 32 00 03 00 00 00 00 00  [.1......2.......]
    80 : 00                                               [.]
object(stdClass)#%d (1) {
  ["stuff"]=>
  array(2) {
    [0]=>
    object(MyClass)#%d (1) {
      ["props"]=>
      array(3) {
        ["__pclass"]=>
        object(MongoDB\BSON\Binary)#%d (2) {
          ["data"]=>
          string(7) "MyClass"
          ["type"]=>
          int(128)
        }
        ["random"]=>
        string(5) "class"
        [0]=>
        string(4) "data"
      }
    }
    [1]=>
    object(MyClass2)#%d (1) {
      ["props"]=>
      array(4) {
        ["__pclass"]=>
        object(MongoDB\BSON\Binary)#%d (2) {
          ["data"]=>
          string(8) "MyClass2"
          ["type"]=>
          int(128)
        }
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
