--TEST--
MongoDB\BSON\Document: Round-trip of BSON with duplicate keys
--DESCRIPTION--
BSON corpus spec prose test #2.3 (round-trip)
https://github.com/mongodb/specifications/blob/master/source/bson-corpus/bson-corpus.md#23-test-round-trip
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* Bytes represent BSON for { "foo": 1, "foo": 2 } */
$bson = hex2bin('1700000010666f6f000100000010666f6f000200000000');
$document = MongoDB\BSON\Document::fromBSON($bson);

echo "Raw BSON is preserved\n";
var_dump((string) $document === $bson);

echo "\nRound-trip through Extended JSON preserves duplicates\n";
$fromJson = MongoDB\BSON\Document::fromJSON($document->toRelaxedExtendedJSON());
echo $fromJson->toRelaxedExtendedJSON(), "\n";
var_dump((string) $fromJson === $bson);

echo "\nRound-trip through PHP keeps the last value\n";
$roundTrip = MongoDB\BSON\Document::fromPHP($document->toPHP());
echo $roundTrip->toRelaxedExtendedJSON(), "\n";
var_dump((string) $roundTrip === $bson);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Raw BSON is preserved
bool(true)

Round-trip through Extended JSON preserves duplicates
{ "foo" : 1, "foo" : 2 }
bool(true)

Round-trip through PHP keeps the last value
{ "foo" : 2 }
bool(false)
===DONE===
