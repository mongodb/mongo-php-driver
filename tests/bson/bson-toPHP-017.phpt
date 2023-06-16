--TEST--
MongoDB\BSON\toPHP(): int64 values are returned as int scalars
--FILE--
<?php

$bson = MongoDB\BSON\fromPHP([
    'int32' => 123,
    // Using a 32-bit value to simplify testing
    'int64' => new MongoDB\BSON\Int64(123),
]);

var_dump(MongoDB\BSON\toPHP($bson));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["int32"]=>
  int(123)
  ["int64"]=>
  int(123)
}
===DONE===
