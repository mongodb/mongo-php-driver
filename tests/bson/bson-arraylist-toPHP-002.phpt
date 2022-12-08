--TEST--
MongoDB\BSON\ArrayList::toPHP(): Use object as root type
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$arrayList = MongoDB\BSON\ArrayList::fromPHP([1, 2, 3]);
var_dump($arrayList->toPHP(['root' => 'object']));

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
