--TEST--
MongoDB\BSON\ArrayList::get() index access
--FILE--
<?php

$child = MongoDB\BSON\ArrayList::fromPHP([(object) ['foo' => 'bar']])->get(0);
var_dump($child);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(24) "APsHAAAAAAAABAAAAGJhcgAA"
  ["length"]=>
  int(18)
}
===DONE===
