--TEST--
MongoDB\BSON\toRelaxedExtendedJSON(): Encoding extended JSON types
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    [ '_id' => new MongoDB\BSON\ObjectId('56315a7c6118fd1b920270b1') ],
    [ 'binary' => new MongoDB\BSON\Binary('foo', MongoDB\BSON\Binary::TYPE_GENERIC) ],
    [ 'date' => new MongoDB\BSON\UTCDateTime(1445990400000) ],
    [ 'timestamp' => new MongoDB\BSON\Timestamp(1234, 5678) ],
    [ 'regex' => new MongoDB\BSON\Regex('pattern', 'i') ],
    [ 'code' => new MongoDB\BSON\Javascript('function() { return 1; }') ],
    [ 'code_ws' => new MongoDB\BSON\Javascript('function() { return a; }', ['a' => 1]) ],
    [ 'minkey' => new MongoDB\BSON\MinKey ],
    [ 'maxkey' => new MongoDB\BSON\MaxKey ],
];

foreach ($tests as $value) {
    $bson = MongoDB\BSON\fromPHP($value);
    echo MongoDB\BSON\toRelaxedExtendedJSON($bson), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toRelaxedExtendedJSON() is deprecated in %s
{ "_id" : { "$oid" : "56315a7c6118fd1b920270b1" } }

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toRelaxedExtendedJSON() is deprecated in %s
{ "binary" : { "$binary" : { "base64" : "Zm9v", "subType" : "00" } } }

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toRelaxedExtendedJSON() is deprecated in %s
{ "date" : { "$date" : "2015-10-28T00:00:00Z" } }

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toRelaxedExtendedJSON() is deprecated in %s
{ "timestamp" : { "$timestamp" : { "t" : 5678, "i" : 1234 } } }

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toRelaxedExtendedJSON() is deprecated in %s
{ "regex" : { "$regularExpression" : { "pattern" : "pattern", "options" : "i" } } }

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toRelaxedExtendedJSON() is deprecated in %s
{ "code" : { "$code" : "function() { return 1; }" } }

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toRelaxedExtendedJSON() is deprecated in %s
{ "code_ws" : { "$code" : "function() { return a; }", "$scope" : { "a" : 1 } } }

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toRelaxedExtendedJSON() is deprecated in %s
{ "minkey" : { "$minKey" : 1 } }

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toRelaxedExtendedJSON() is deprecated in %s
{ "maxkey" : { "$maxKey" : 1 } }
===DONE===
