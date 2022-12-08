--TEST--
MongoDB\BSON\ArrayList::get() index access
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$document = MongoDB\BSON\ArrayList::fromPHP([0, 1, "foo"]);
var_dump($document->get(0));
var_dump($document->get(2));

echo throws(function() use ($document) {
    var_dump($document->get(3));
}, MongoDB\Driver\Exception\RuntimeException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
int(0)
string(3) "foo"
OK: Got MongoDB\Driver\Exception\RuntimeException
Could not find index "3" in BSON data
===DONE===
