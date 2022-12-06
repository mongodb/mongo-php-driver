--TEST--
MongoDB\BSON\Iterator can work with discarded objects
--FILE--
<?php

$document = MongoDB\BSON\Document::fromPHP(['document' => (object) ['foo' => 'bar']]);
$iterator = $document->getIterator();
unset($document);
var_dump(iterator_to_array($iterator));

$array = MongoDB\BSON\Document::fromPHP([0, 1, "foo"]);
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
    string(18) "%s"
    ["length"]=>
    int(18)
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