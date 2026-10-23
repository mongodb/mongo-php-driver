--TEST--
MongoDB\BSON\Document: Parsing Extended JSON with duplicate keys
--DESCRIPTION--
BSON corpus spec prose test #2.4 (parsing Extended JSON)
https://github.com/mongodb/specifications/blob/master/source/bson-corpus/bson-corpus.md#24-test-parsing-extended-json
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* Bytes represent BSON for { "foo": 1, "foo": 2 } */
$expected = hex2bin('1700000010666f6f000100000010666f6f000200000000');

$document = MongoDB\BSON\Document::fromJSON('{ "foo": 1, "foo": 2 }');

echo "Duplicate keys are preserved in order\n";
echo $document->toRelaxedExtendedJSON(), "\n";
var_dump((string) $document === $expected);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Duplicate keys are preserved in order
{ "foo" : 1, "foo" : 2 }
bool(true)
===DONE===
