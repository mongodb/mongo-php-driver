--TEST--
MongoDB\BSON\ArrayList::get() index access returns BSON structures
--FILE--
<?php

$array = MongoDB\BSON\ArrayList::fromPHP([(object) ['foo' => 'bar']]);
$child = $array->get(0);
unset($array);
var_dump($child->toPHP());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["foo"]=>
  string(3) "bar"
}
===DONE===
