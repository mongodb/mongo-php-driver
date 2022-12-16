--TEST--
MongoDB\BSON\Document comparisons
--FILE--
<?php

$foobar = MongoDB\BSON\Document::fromPHP(['foo' => 1, 'bar' => 2]);
$barfoo = MongoDB\BSON\Document::fromPHP(['bar' => 1, 'foo' => 2]);

var_dump($foobar == $barfoo);
var_dump($foobar < $barfoo);
var_dump($foobar > $barfoo);

var_dump($foobar == $foobar);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(false)
bool(false)
bool(true)
bool(true)
===DONE===
