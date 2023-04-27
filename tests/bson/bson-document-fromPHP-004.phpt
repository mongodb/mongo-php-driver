--TEST--
MongoDB\BSON\Document::fromPHP() copies BSON data from Document and PackedArray
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$document = MongoDB\BSON\Document::fromJSON('{ "foo": "bar" }');
$fromDocument = MongoDB\BSON\Document::fromPHP($document);
echo $fromDocument->toRelaxedExtendedJSON(), "\n";

// This will be interpreted as an object after copying, i.e. { "0": 1, "1": 2, "2": 3 }
$packedArray = MongoDB\BSON\PackedArray::fromPHP([ 1, 2, 3 ]);
$fromPackedArray = MongoDB\BSON\Document::fromPHP($packedArray);
echo $fromPackedArray->toRelaxedExtendedJSON(), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
{ "foo" : "bar" }
{ "0" : 1, "1" : 2, "2" : 3 }
===DONE===
