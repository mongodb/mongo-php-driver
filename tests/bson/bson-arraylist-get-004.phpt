--TEST--
MongoDB\BSON\ArrayList::get tests with invalid key
--FILE--
<?php

$degenerateBson = hex2bin('150000000461000D000000106162000A0000000000');

$arrayList = MongoDB\BSON\Document::fromBSON($degenerateBson)->get('a');
var_dump($arrayList->has(0));
var_dump($arrayList->get(0));
var_dump($arrayList->toPHP());
var_dump(iterator_to_array($arrayList));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
int(10)
array(1) {
  [0]=>
  int(10)
}
array(1) {
  [0]=>
  int(10)
}
===DONE===
