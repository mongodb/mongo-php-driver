--TEST--
MongoDB\BSON\Iterator maintains reference to its yielding Document or PackedArray
--FILE--
<?php

$document = MongoDB\BSON\Document::fromPHP(['document' => (object) ['foo' => 'bar']]);
$iterator = $document->getIterator();
unset($document);
var_dump(iterator_to_array($iterator));

$array = MongoDB\BSON\PackedArray::fromPHP([0, 1, "foo"]);
$iterator = $array->getIterator();
unset($array);
var_dump(iterator_to_array($iterator));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(1) {
  ["document"]=>
  object(MongoDB\BSON\Document)#%d (%d) {
    ["data"]=>
    string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
    ["value"]=>
    object(stdClass)#%d (%d) {
      ["foo"]=>
      string(3) "bar"
    }
  }
}
array(3) {
  [0]=>
  int(0)
  [1]=>
  int(1)
  [2]=>
  string(3) "foo"
}
===DONE===
