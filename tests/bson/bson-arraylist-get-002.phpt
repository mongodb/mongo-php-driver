--TEST--
MongoDB\BSON\ArrayList::get() index access returns BSON structures
--FILE--
<?php

$array = MongoDB\BSON\ArrayList::fromPHP([(object) ['foo' => 'bar']]);
$child = $array->get(0);
unset($array);
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
