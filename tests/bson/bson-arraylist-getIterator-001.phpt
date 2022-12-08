--TEST--
MongoDB\BSON\ArrayList::getIterator() tests
--FILE--
<?php

$iterator = MongoDB\BSON\ArrayList::fromPHP([1, 2])->getIterator();

echo get_class($iterator), "\n";

foreach ($iterator as $key => $value) {
    var_dump($key);
    var_dump($value);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
MongoDB\BSON\Iterator
int(0)
int(1)
int(1)
int(2)
===DONE===
