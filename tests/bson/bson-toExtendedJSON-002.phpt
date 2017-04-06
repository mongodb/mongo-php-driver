--TEST--
MongoDB\BSON\toExtendedJSON(): Encoding extended JSON types
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    ['_id' => new MongoDB\BSON\ObjectId('56315a7c6118fd1b920270b1')],
    ['binary' => new MongoDB\BSON\Binary('foo', MongoDB\BSON\Binary::TYPE_GENERIC)],
    ['date' => new MongoDB\BSON\UTCDateTime(1445990400000)],
    ['timestamp' => new MongoDB\BSON\Timestamp(MongoDB\BSON\Binary::TYPE_GENERIC, 1446084619)],
    ['regex' => new MongoDB\BSON\Regex('pattern', 'i')],
    ['minkey' => new MongoDB\BSON\MinKey],
    ['maxkey' => new MongoDB\BSON\MaxKey],
    ['long' => 1234],
];

foreach ($tests as $value) {
    $bson = fromPHP($value);
    echo toExtendedJSON($bson), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
{ "_id" : { "$oid" : "56315a7c6118fd1b920270b1" } }
{ "binary" : { "$binary" : "Zm9v", "$type" : "00" } }
{ "date" : { "$date" : { "$numberLong" : "1445990400000" } } }
{ "timestamp" : { "$timestamp" : "6210886145853620224" } }
{ "regex" : { "$regex" : "pattern", "$options" : "i" } }
{ "minkey" : { "$minKey" : 1 } }
{ "maxkey" : { "$maxKey" : 1 } }
{ "long" : { "$numberInt" : "1234" } }
===DONE===
