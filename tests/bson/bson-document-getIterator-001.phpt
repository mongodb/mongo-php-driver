--TEST--
MongoDB\BSON\Document::getIterator() tests
--FILE--
<?php

$tests = [
    '{ "_id": { "$oid": "56315a7c6118fd1b920270b1" }}',
    '{ "binary": { "$binary": "Zm9v", "$type": "00" }}',
    '{ "date": { "$date": "2015-10-28T00:00:00Z" }}',
    '{ "timestamp": { "$timestamp": { "t": 1446084619, "i": 0 }}}',
    '{ "regex": { "$regex": "pattern", "$options": "i" }}',
    '{ "undef": { "$undefined": true }}',
    '{ "minkey": { "$minKey": 1 }}',
    '{ "maxkey": { "$maxKey": 1 }}',
    '{ "long": { "$numberLong": "1234" }}',
    '{ "array": [1, 2, "foo"]}',
    '{ "document": { "foo": "bar"}}',
];

foreach ($tests as $test) {
    echo 'Test ', $test, "\n";
    $iterator = MongoDB\BSON\Document::fromJSON($test)->getIterator();
    var_dump($iterator->key());
    $current = $iterator->current();
    var_dump($current);

    if ($current instanceof MongoDB\BSON\Document || $current instanceof MongoDB\BSON\ArrayList) {
        var_dump($current->toPHP());
    }
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Test { "_id": { "$oid": "56315a7c6118fd1b920270b1" }}
string(3) "_id"
object(MongoDB\BSON\ObjectId)#%d (%d) {
  ["oid"]=>
  string(24) "56315a7c6118fd1b920270b1"
}
Test { "binary": { "$binary": "Zm9v", "$type": "00" }}
string(6) "binary"
object(MongoDB\BSON\Binary)#%d (%d) {
  ["data"]=>
  string(3) "foo"
  ["type"]=>
  int(0)
}
Test { "date": { "$date": "2015-10-28T00:00:00Z" }}
string(4) "date"
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(13) "1445990400000"
}
Test { "timestamp": { "$timestamp": { "t": 1446084619, "i": 0 }}}
string(9) "timestamp"
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  string(1) "0"
  ["timestamp"]=>
  string(10) "1446084619"
}
Test { "regex": { "$regex": "pattern", "$options": "i" }}
string(5) "regex"
object(MongoDB\BSON\Regex)#%d (%d) {
  ["pattern"]=>
  string(7) "pattern"
  ["flags"]=>
  string(1) "i"
}
Test { "undef": { "$undefined": true }}
string(5) "undef"
object(MongoDB\BSON\Undefined)#%d (%d) {
}
Test { "minkey": { "$minKey": 1 }}
string(6) "minkey"
object(MongoDB\BSON\MinKey)#%d (%d) {
}
Test { "maxkey": { "$maxKey": 1 }}
string(6) "maxkey"
object(MongoDB\BSON\MaxKey)#%d (%d) {
}
Test { "long": { "$numberLong": "1234" }}
string(4) "long"
int(1234)
Test { "array": [1, 2, "foo"]}
string(5) "array"
object(MongoDB\BSON\ArrayList)#%d (%d) {
  ["data"]=>
  string(40) "HgAAABAwAAEAAAAQMQACAAAAAjIABAAAAGZvbwAA"
}
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  string(3) "foo"
}
Test { "document": { "foo": "bar"}}
string(8) "document"
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
}
object(stdClass)#%d (%d) {
  ["foo"]=>
  string(3) "bar"
}
===DONE===
