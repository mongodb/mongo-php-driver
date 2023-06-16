--TEST--
MongoDB\BSON\Document::toPHP(): int64 values are returned as Int64 instances
--FILE--
<?php

$bson = MongoDB\BSON\Document::fromPHP([
    'int32' => 123,
    'int64' => new MongoDB\BSON\Int64(123),
]);

var_dump($bson->toPHP());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["int32"]=>
  int(123)
  ["int64"]=>
  object(MongoDB\BSON\Int64)#%d (1) {
    ["integer"]=>
    string(3) "123"
  }
}
===DONE===
