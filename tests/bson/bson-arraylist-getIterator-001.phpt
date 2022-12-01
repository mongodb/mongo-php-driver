--TEST--
MongoDB\BSON\ArrayList::getIterator tests
--FILE--
<?php

$array = [
    MongoDB\BSON\Document::fromJSON('{ "_id": { "$oid": "56315a7c6118fd1b920270b1" }}'),
    MongoDB\BSON\Document::fromJSON('{ "binary": { "$binary": "Zm9v", "$type": "00" }}'),
    MongoDB\BSON\Document::fromJSON('{ "date": { "$date": "2015-10-28T00:00:00Z" }}'),
    MongoDB\BSON\Document::fromJSON('{ "timestamp": { "$timestamp": { "t": 1446084619, "i": 0 }}}'),
    MongoDB\BSON\Document::fromJSON('{ "regex": { "$regex": "pattern", "$options": "i" }}'),
    MongoDB\BSON\Document::fromJSON('{ "undef": { "$undefined": true }}'),
    MongoDB\BSON\Document::fromJSON('{ "minkey": { "$minKey": 1 }}'),
    MongoDB\BSON\Document::fromJSON('{ "maxkey": { "$maxKey": 1 }}'),
    MongoDB\BSON\Document::fromJSON('{ "long": { "$numberLong": "1234" }}'),
    MongoDB\BSON\Document::fromJSON('{ "document": { "foo": "bar"}}'),
    [1, 2, "foo"],
    MongoDB\BSON\ArrayList::fromPHP([1, 2, "foo"]),
];

$iterator = MongoDB\BSON\ArrayList::fromPHP($array)->getIterator();

foreach ($iterator as $key => $value) {
    var_dump($key);

    if ($value instanceof MongoDB\BSON\Document || $value instanceof MongoDB\BSON\ArrayList) {
        var_dump(get_class($value));
        var_dump($value->toPHP());
    } else {
        var_dump($value);
    }
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
int(0)
string(21) "MongoDB\BSON\Document"
object(stdClass)#%d (%d) {
  ["_id"]=>
  object(MongoDB\BSON\ObjectId)#%d (%d) {
    ["oid"]=>
    string(24) "56315a7c6118fd1b920270b1"
  }
}
int(1)
string(21) "MongoDB\BSON\Document"
object(stdClass)#%d (%d) {
  ["binary"]=>
  object(MongoDB\BSON\Binary)#%d (%d) {
    ["data"]=>
    string(3) "foo"
    ["type"]=>
    int(0)
  }
}
int(2)
string(21) "MongoDB\BSON\Document"
object(stdClass)#%d (%d) {
  ["date"]=>
  object(MongoDB\BSON\UTCDateTime)#%d (%d) {
    ["milliseconds"]=>
    string(13) "1445990400000"
  }
}
int(3)
string(21) "MongoDB\BSON\Document"
object(stdClass)#%d (%d) {
  ["timestamp"]=>
  object(MongoDB\BSON\Timestamp)#%d (2) {
    ["increment"]=>
    string(1) "0"
    ["timestamp"]=>
    string(10) "1446084619"
  }
}
int(4)
string(21) "MongoDB\BSON\Document"
object(stdClass)#%d (%d) {
  ["regex"]=>
  object(MongoDB\BSON\Regex)#%d (2) {
    ["pattern"]=>
    string(7) "pattern"
    ["flags"]=>
    string(1) "i"
  }
}
int(5)
string(21) "MongoDB\BSON\Document"
object(stdClass)#%d (%d) {
  ["undef"]=>
  object(MongoDB\BSON\Undefined)#%d (%d) {
  }
}
int(6)
string(21) "MongoDB\BSON\Document"
object(stdClass)#%d (%d) {
  ["minkey"]=>
  object(MongoDB\BSON\MinKey)#%d (%d) {
  }
}
int(7)
string(21) "MongoDB\BSON\Document"
object(stdClass)#%d (%d) {
  ["maxkey"]=>
  object(MongoDB\BSON\MaxKey)#%d (%d) {
  }
}
int(8)
string(21) "MongoDB\BSON\Document"
object(stdClass)#%d (%d) {
  ["long"]=>
  int(1234)
}
int(9)
string(21) "MongoDB\BSON\Document"
object(stdClass)#%d (%d) {
  ["document"]=>
  object(stdClass)#%d (%d) {
    ["foo"]=>
    string(3) "bar"
  }
}
int(10)
string(22) "MongoDB\BSON\ArrayList"
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  string(3) "foo"
}
int(11)
string(22) "MongoDB\BSON\ArrayList"
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  string(3) "foo"
}
===DONE===
