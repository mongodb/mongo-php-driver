--TEST--
MongoDB\BSON\Iterator is rewound when cloning
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$iterator = MongoDB\BSON\Document::fromJSON('[ 1, 2, 3 ]')->getIterator();
$iterator->next();

$clone = clone $iterator;

var_dump($iterator->current());
var_dump($clone->current());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
int(2)
int(1)
===DONE===
