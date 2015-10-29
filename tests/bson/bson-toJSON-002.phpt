--TEST--
BSON\toJSON(): Encoding extended JSON types
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
use MongoDB\BSON as BSON;

require_once __DIR__ . "/../utils/basic.inc";

$tests = [
    ['_id' => new BSON\ObjectId('56315a7c6118fd1b920270b1')],
    ['binary' => new BSON\Binary('foo', BSON\Binary::TYPE_GENERIC)],
    ['date' => new BSON\UTCDateTime(1445990400000)],
    ['timestamp' => new BSON\Timestamp(BSON\Binary::TYPE_GENERIC, 1446084619)],
    ['regex' => new BSON\Regex('pattern', 'i')],
    ['minkey' => new BSON\MinKey],
    ['maxkey' => new BSON\MaxKey],
    ['long' => 1234],
];

foreach ($tests as $value) {
    $bson = fromPHP($value);
    echo toJSON($bson), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
{ "_id" : { "$oid" : "56315a7c6118fd1b920270b1" } }
{ "binary" : { "$type" : "00", "$binary" : "Zm9v" } }
{ "date" : { "$date" : 1445990400000 } }
{ "timestamp" : { "$timestamp" : { "t" : 1446084619, "i" : 0 } } }
{ "regex" : { "$regex" : "pattern", "$options" : "i" } }
{ "minkey" : { "$minKey" : 1 } }
{ "maxkey" : { "$maxKey" : 1 } }
{ "long" : 1234 }
===DONE===
