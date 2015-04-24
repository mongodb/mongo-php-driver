--TEST--
BSON encoding: Object Document Mapper
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$data = array(
    "emptyarray" => array(),
    "emptyclass" => new stdclass,
);

$s = BSON\fromArray($data);
echo "Test ", BSON\toJSON($s), "\n";
hex_dump($s);
$ret = BSON\toArray($s);
var_dump($ret);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Test { "emptyarray" : [  ], "emptyclass" : {  } }
     0 : 27 00 00 00 04 65 6d 70 74 79 61 72 72 61 79 00  ['....emptyarray.]
    10 : 05 00 00 00 00 03 65 6d 70 74 79 63 6c 61 73 73  [......emptyclass]
    20 : 00 05 00 00 00 00 00                             [.......]
array(2) {
  ["emptyarray"]=>
  array(0) {
  }
  ["emptyclass"]=>
  object(stdClass)#%d (0) {
  }
}
===DONE===
