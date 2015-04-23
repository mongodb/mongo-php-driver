--TEST--
BSON Serializing a PHP resource should throw exception
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

try {
    $a = array("stderr" => STDERR);

    $b = BSON\fromArray($a);
} catch(MongoDB\Driver\UnexpectedValueException $e) {
    echo $e->getMessage(), "\n";
}
var_dump($a);

try {
    $a = array("stderr" => STDERR, "stdout" => STDOUT);

    $b = BSON\fromArray($a);
} catch(MongoDB\Driver\UnexpectedValueException $e) {
    echo $e->getMessage(), "\n";
}
var_dump($a);


?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Got unsupported type 'resource'
array(1) {
  ["stderr"]=>
  resource(3) of type (stream)
}
Got unsupported type 'resource'
array(2) {
  ["stderr"]=>
  resource(3) of type (stream)
  ["stdout"]=>
  resource(2) of type (stream)
}
===DONE===
