--TEST--
MongoDB\BSON\Document array access with integers (ArrayAccess methods)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$document = MongoDB\BSON\Document::fromPHP([
    '0' => 'foo',
    '1' => 'bar',
]);

// Use a variable to assert that conversion doesn't affect the original zval
$key = 1;

var_dump($document->offsetExists(0));
var_dump($document->offsetExists($key));
var_dump($document->offsetExists(2));

var_dump($document->offsetGet(0));
var_dump($document->offsetGet($key));

var_dump($key);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
bool(false)
string(3) "foo"
string(3) "bar"
int(1)
===DONE===
