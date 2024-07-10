--TEST--
MongoDB\BSON\Symbol get_properties handler (get_object_vars)
--FILE--
<?php

$document = MongoDB\BSON\Document::fromJSON('{"symbol":{"$symbol":"test"}}')->toPHP();
$symbol = $document->symbol;

var_dump(get_object_vars($symbol));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(1) {
  ["symbol"]=>
  string(4) "test"
}
===DONE===
