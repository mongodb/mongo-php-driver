--TEST--
MongoDB\BSON\ArrayList::get tests
--FILE--
<?php

$child = MongoDB\BSON\ArrayList::fromPHP([(object) ['foo' => 'bar']])->get(0);
var_dump($child);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\BSONDocument)#%d (%d) {
  ["data"]=>
  string(18) "%s"
  ["length"]=>
  int(18)
}
===DONE===
