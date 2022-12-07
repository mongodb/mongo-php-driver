--TEST--
MongoDB\BSON\ArrayList::get() index access
--FILE--
<?php

$child = MongoDB\BSON\ArrayList::fromPHP([(object) ['foo' => 'bar']])->get(0);
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
