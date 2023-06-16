--TEST--
MongoDB\BSON\PackedArray::get() fixes invalid keys
--FILE--
<?php

$degenerateBson = hex2bin('150000000461000D000000106162000A0000000000');

$packedArray = MongoDB\BSON\Document::fromBSON($degenerateBson)->get('a');
var_dump($packedArray->has(0));
var_dump($packedArray->get(0));
var_dump($packedArray->toPHP());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
int(10)
array(1) {
  [0]=>
  int(10)
}
===DONE===
