--TEST--
MongoDB\BSON\Document: Decoding BSON with duplicate keys
--DESCRIPTION--
BSON corpus spec prose test #2.2 (decoding)
https://github.com/mongodb/specifications/blob/master/source/bson-corpus/bson-corpus.md#22-test-decoding
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* Duplicate keys are not validated by the driver. This test documents how each
 * decoding API behaves. Bytes represent BSON for { "foo": 1, "foo": 2 } */
$document = MongoDB\BSON\Document::fromBSON(hex2bin('1700000010666f6f000100000010666f6f000200000000'));

echo "Iteration returns both elements in order\n";
foreach ($document as $key => $value) {
    var_dump($key, $value);
}

echo "\nget() and offsetGet() return the first value\n";
var_dump($document->get('foo'));
var_dump($document['foo']);
var_dump($document->has('foo'));

echo "\ntoPHP() keeps the last value for object type maps\n";
var_dump($document->toPHP());
var_dump($document->toPHP(['root' => 'object']));

echo "\ntoPHP() keeps the last value for array type maps\n";
var_dump($document->toPHP(['root' => 'array']));

echo "\ntoPHP() with the bson type map preserves duplicates\n";
$bsonDocument = $document->toPHP(['root' => 'bson']);
var_dump($bsonDocument instanceof MongoDB\BSON\Document);
echo $bsonDocument->toRelaxedExtendedJSON(), "\n";

echo "\nExtended JSON preserves duplicates\n";
echo $document->toRelaxedExtendedJSON(), "\n";
echo $document->toCanonicalExtendedJSON(), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Iteration returns both elements in order
string(3) "foo"
int(1)
string(3) "foo"
int(2)

get() and offsetGet() return the first value
int(1)
int(1)
bool(true)

toPHP() keeps the last value for object type maps
object(stdClass)#%d (1) {
  ["foo"]=>
  int(2)
}
object(stdClass)#%d (1) {
  ["foo"]=>
  int(2)
}

toPHP() keeps the last value for array type maps
array(1) {
  ["foo"]=>
  int(2)
}

toPHP() with the bson type map preserves duplicates
bool(true)
{ "foo" : 1, "foo" : 2 }

Extended JSON preserves duplicates
{ "foo" : 1, "foo" : 2 }
{ "foo" : { "$numberInt" : "1" }, "foo" : { "$numberInt" : "2" } }
===DONE===
