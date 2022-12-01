--TEST--
MongoDB\BSON\ArrayList::toPHP(): Convert BSON array to PHP array
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$arrayList = MongoDB\BSON\ArrayList::fromPHP([1, 2, 3]);
var_dump($arrayList->toPHP(['root' => stdClass::class])); // stdClass is ignored

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
===DONE===
