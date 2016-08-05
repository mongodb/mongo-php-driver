--TEST--
BSON encoding: Object Document Mapper
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$data = array(
    "emptyarray" => array(),
    "emptyclass" => new stdclass,
);

$s = fromPHP($data);
echo "Test ", toJSON($s), "\n";
hex_dump($s);
$ret = toPHP($s);
var_dump($ret);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Test { "emptyarray" : [  ], "emptyclass" : {  } }
     0 : 27 00 00 00 04 65 6d 70 74 79 61 72 72 61 79 00  ['....emptyarray.]
    10 : 05 00 00 00 00 03 65 6d 70 74 79 63 6c 61 73 73  [......emptyclass]
    20 : 00 05 00 00 00 00 00                             [.......]
object(stdClass)#%d (2) {
  ["emptyarray"]=>
  array(0) {
  }
  ["emptyclass"]=>
  object(stdClass)#%d (0) {
  }
}
===DONE===
