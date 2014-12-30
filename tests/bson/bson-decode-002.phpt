--TEST--
BSON encoding: Encoding object/arrays data into user specificied classes
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php 
require_once "tests/utils/basic.inc";

$tests = array(
    array(array("hello" => "world")),
    array((object)array("hello" => "world")),
    array("my" => array("hello" => "world")),
    array("my" => (object)array("hello" => "world")),
    array("my" => array(array("hello", "world"))),
    array("my" => (object)array(array("hello", "world"))),
);

foreach($tests as $n => $test) {
    $s = BSON\fromArray($test);
    echo "Test#{$n} ", BSON\toJSON($s), "\n";
    $val = BSON\toArray($s, "ArrayObject");
    var_dump($val);
}
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Test#%d { "0" : { "hello" : "world" } }
array(1) {
  [0]=>
  object(ArrayObject)#%d (1) {
    [%s]=>
    array(1) {
      ["hello"]=>
      string(5) "world"
    }
  }
}
Test#%d { "0" : { "hello" : "world" } }
array(1) {
  [0]=>
  object(ArrayObject)#%d (1) {
    [%s]=>
    array(1) {
      ["hello"]=>
      string(5) "world"
    }
  }
}
Test#%d { "my" : { "hello" : "world" } }
array(1) {
  ["my"]=>
  object(ArrayObject)#%d (1) {
    [%s]=>
    array(1) {
      ["hello"]=>
      string(5) "world"
    }
  }
}
Test#%d { "my" : { "hello" : "world" } }
array(1) {
  ["my"]=>
  object(ArrayObject)#%d (1) {
    [%s]=>
    array(1) {
      ["hello"]=>
      string(5) "world"
    }
  }
}
Test#%d { "my" : [ [ "hello", "world" ] ] }
array(1) {
  ["my"]=>
  object(ArrayObject)#%d (1) {
    [%s]=>
    array(1) {
      [0]=>
      object(ArrayObject)#%d (1) {
        [%s]=>
        array(2) {
          [0]=>
          string(5) "hello"
          [1]=>
          string(5) "world"
        }
      }
    }
  }
}
Test#%d { "my" : { "0" : [ "hello", "world" ] } }
array(1) {
  ["my"]=>
  object(ArrayObject)#%d (1) {
    [%s]=>
    array(1) {
      [0]=>
      object(ArrayObject)#%d (1) {
        [%s]=>
        array(2) {
          [0]=>
          string(5) "hello"
          [1]=>
          string(5) "world"
        }
      }
    }
  }
}
===DONE===
