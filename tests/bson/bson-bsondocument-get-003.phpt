--TEST--
MongoDB\BSON\BSONDocument::get tests
--FILE--
<?php

$child = MongoDB\BSON\BSONDocument::fromPHP(['document' => (object) ['foo' => 'bar']])->get('document');
var_dump($child);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\BSONDocument)#%d (%d) {
  ["data"]=>
  string(18) "%s"
  ["length"]=>
  int(18)
}
===DONE===
