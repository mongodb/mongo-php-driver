--TEST--
MongoDB\BSON\ArrayList::get tests
--FILE--
<?php

$document = MongoDB\BSON\ArrayList::fromPHP([(object) ['foo' => 'bar']]);
$child = $document->get(0);
unset($document);
var_dump($child);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(18) "%s"
  ["length"]=>
  int(18)
}
===DONE===