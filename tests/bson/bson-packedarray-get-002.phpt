--TEST--
MongoDB\BSON\PackedArray::get() index access returns BSON structures
--FILE--
<?php

$array = MongoDB\BSON\PackedArray::fromPHP([(object) ['foo' => 'bar']]);
$child = $array->get(0);
unset($array);
var_dump($child);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
  ["value"]=>
  object(stdClass)#%d (%d) {
    ["foo"]=>
    string(3) "bar"
  }
}
===DONE===
