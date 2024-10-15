--TEST--
MongoDB\BSON\PackedArray::toCanonicalExtendedJSON(): Encoding extended JSON types
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    [new MongoDB\BSON\ObjectId('56315a7c6118fd1b920270b1') ],
    [new MongoDB\BSON\Binary('foo', MongoDB\BSON\Binary::TYPE_GENERIC) ],
    [new MongoDB\BSON\UTCDateTime(new MongoDB\BSON\Int64('1445990400000')) ],
    [new MongoDB\BSON\Timestamp(1234, 5678) ],
    [new MongoDB\BSON\Regex('pattern', 'i') ],
    [new MongoDB\BSON\Javascript('function() { return 1; }') ],
    [new MongoDB\BSON\Javascript('function() { return a; }', ['a' => 1]) ],
    [new MongoDB\BSON\MinKey ],
    [new MongoDB\BSON\MaxKey ],
];

foreach ($tests as $value) {
    echo MongoDB\BSON\PackedArray::fromPHP($value)->toCanonicalExtendedJSON(), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
[ { "$oid" : "56315a7c6118fd1b920270b1" } ]
[ { "$binary" : { "base64" : "Zm9v", "subType" : "00" } } ]
[ { "$date" : { "$numberLong" : "1445990400000" } } ]
[ { "$timestamp" : { "t" : 5678, "i" : 1234 } } ]
[ { "$regularExpression" : { "pattern" : "pattern", "options" : "i" } } ]
[ { "$code" : "function() { return 1; }" } ]
[ { "$code" : "function() { return a; }", "$scope" : { "a" : { "$numberInt" : "1" } } } ]
[ { "$minKey" : 1 } ]
[ { "$maxKey" : 1 } ]
===DONE===
