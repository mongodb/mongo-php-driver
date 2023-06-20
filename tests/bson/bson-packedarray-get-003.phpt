--TEST--
MongoDB\BSON\PackedArray::get() index access
--FILE--
<?php

$child = MongoDB\BSON\PackedArray::fromPHP([(object) ['foo' => 'bar']])->get(0);
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
