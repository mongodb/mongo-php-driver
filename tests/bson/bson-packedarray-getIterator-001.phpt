--TEST--
MongoDB\BSON\PackedArray::getIterator() tests
--FILE--
<?php

$iterator = MongoDB\BSON\PackedArray::fromPHP([1, 2])->getIterator();

echo get_class($iterator), "\n";

foreach ($iterator as $key => $value) {
    var_dump($key);
    var_dump($value);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
MongoDB\BSON\Iterator
int(0)
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(14) "32-bit integer"
  ["value"]=>
  int(1)
}
int(1)
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(14) "32-bit integer"
  ["value"]=>
  int(2)
}
===DONE===
