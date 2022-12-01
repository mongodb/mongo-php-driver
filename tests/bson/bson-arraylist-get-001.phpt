--TEST--
MongoDB\BSON\ArrayList::get tests
--FILE--
<?php

$document = MongoDB\BSON\ArrayList::fromPHP([0, 1, "foo"]);
var_dump($document->get(0));
var_dump($document->get(2));
var_dump($document->get(3));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
int(0)
string(3) "foo"
NULL
===DONE===
