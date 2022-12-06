--TEST--
MongoDB\BSON\ArrayList::get tests with duplicate key
--FILE--
<?php

$degenerateBson = hex2bin('1b000000046100130000001030000a000000103000140000000000');

$arrayList = MongoDB\BSON\Document::fromBSON($degenerateBson)->get('a');
var_dump($arrayList->has(0));
var_dump($arrayList->get(0));
var_dump($arrayList->has(1));
var_dump($arrayList->get(1));
var_dump($arrayList->toPHP());
var_dump(iterator_to_array($arrayList));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
int(10)
bool(true)
int(20)
array(2) {
  [0]=>
  int(10)
  [1]=>
  int(20)
}
array(2) {
  [0]=>
  int(10)
  [1]=>
  int(20)
}
===DONE===
