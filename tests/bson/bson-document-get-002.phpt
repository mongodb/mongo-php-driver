--TEST--
MongoDB\BSON\Document::get() key access
--FILE--
<?php

$document = MongoDB\BSON\Document::fromPHP(['document' => (object) ['foo' => 'bar']]);
$child = $document->get('document');
unset($document);
var_dump($child);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(24) "AAAAAAJmb28ABAAAAGJhcgAA"
  ["length"]=>
  int(18)
}
===DONE===
