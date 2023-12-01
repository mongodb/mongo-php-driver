--TEST--
MongoDB\BSON\Document array access (coalesce operator)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$document = MongoDB\BSON\Document::fromPHP([]);

var_dump(isset($document['foo']));
var_dump($document['foo'] ?? 'fallback');

var_dump(isset($document[0]));
var_dump($document[0] ?? 'fallback');

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(false)
string(8) "fallback"
bool(false)
string(8) "fallback"
===DONE===
