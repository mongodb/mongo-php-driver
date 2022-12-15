--TEST--
MongoDB\BSON\PackedArray::toPHP(): Use object as root type
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$packedArray = MongoDB\BSON\PackedArray::fromPHP([1, 2, 3]);
var_dump($packedArray->toPHP(['root' => 'object']));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["0"]=>
  int(1)
  ["1"]=>
  int(2)
  ["2"]=>
  int(3)
}
===DONE===
