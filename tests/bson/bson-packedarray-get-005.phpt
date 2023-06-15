--TEST--
MongoDB\BSON\PackedArray::get() fixes duplicate keys
--FILE--
<?php

$degenerateBson = hex2bin('1b000000046100130000001030000a000000103000140000000000');

$packedArray = MongoDB\BSON\Document::fromBSON($degenerateBson)->get('a')->getArray();
var_dump($packedArray->has(0));
var_dump($packedArray->get(0));
var_dump($packedArray->has(1));
var_dump($packedArray->get(1));
var_dump($packedArray->toPHP());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(14) "32-bit integer"
  ["value"]=>
  int(10)
}
bool(true)
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(14) "32-bit integer"
  ["value"]=>
  int(20)
}
array(2) {
  [0]=>
  int(10)
  [1]=>
  int(20)
}
===DONE===
