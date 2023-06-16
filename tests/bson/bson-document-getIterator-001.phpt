--TEST--
MongoDB\BSON\Document::getIterator() tests
--FILE--
<?php

$iterator = MongoDB\BSON\Document::fromPHP(['foo' => 'bar', 'bar' => 'baz'])->getIterator();

echo get_class($iterator), "\n";

foreach ($iterator as $key => $value) {
    var_dump($key);
    var_dump($value);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
MongoDB\BSON\Iterator
string(3) "foo"
string(3) "bar"
string(3) "bar"
string(3) "baz"
===DONE===
