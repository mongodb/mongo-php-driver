--TEST--
MongoDB\BSON\BSONArray::has tests
--FILE--
<?php

$document = MongoDB\BSON\BSONArray::fromPHP([0, 1, "foo"]);
var_dump($document->has(0));
var_dump($document->has(2));
var_dump($document->has(3));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
bool(false)
===DONE===
