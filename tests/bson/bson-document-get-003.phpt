--TEST--
MongoDB\BSON\Document::get() key access
--FILE--
<?php

$child = MongoDB\BSON\Document::fromPHP(['document' => (object) ['foo' => 'bar']])->get('document');
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
