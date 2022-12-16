--TEST--
MongoDB\BSON\Document::get() key access
--FILE--
<?php

$document = MongoDB\BSON\Document::fromPHP(['document' => (object) ['foo' => 'bar']]);
$child = $document->get('document');
unset($document);
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
