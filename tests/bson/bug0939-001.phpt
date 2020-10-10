--TEST--
PHPC-939: BSON classes should not assign public properties after var_dump()
--FILE--
<?php

$tests = [
    [ new MongoDB\BSON\Binary('foo', MongoDB\BSON\Binary::TYPE_GENERIC), ['data', 'type'] ],
    [ new MongoDB\BSON\Decimal128('3.14'), ['dec'] ],
    [ new MongoDB\BSON\Javascript('function foo() { return bar; }', ['bar' => 42]), ['code', 'scope'] ],
    [ new MongoDB\BSON\MaxKey, [] ],
    [ new MongoDB\BSON\MinKey, [] ],
    [ new MongoDB\BSON\ObjectId, ['oid'] ],
    [ new MongoDB\BSON\Regex('foo', 'i'), ['pattern', 'flags'] ],
    [ new MongoDB\BSON\Timestamp(1234, 5678), ['increment', 'timestamp'] ],
    [ new MongoDB\BSON\UTCDateTime, ['milliseconds'] ],
];

foreach ($tests as $test) {
    list($object, $properties) = $test;

    var_dump($object);

    foreach ($properties as $property) {
        printf("%s::$%s exists: %s\n", get_class($object), $property, property_exists($object, $property) ? 'yes' : 'no');
    }

    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--

object(MongoDB\BSON\Binary)#%d (%d) {
  ["data"]=>
  string(3) "foo"
  ["type"]=>
  int(0)
}
MongoDB\BSON\Binary::$data exists: no
MongoDB\BSON\Binary::$type exists: no

object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(4) "3.14"
}
MongoDB\BSON\Decimal128::$dec exists: no

object(MongoDB\BSON\Javascript)#%d (%d) {
  ["code"]=>
  string(30) "function foo() { return bar; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
    ["bar"]=>
    int(42)
  }
}
MongoDB\BSON\Javascript::$code exists: no
MongoDB\BSON\Javascript::$scope exists: no

object(MongoDB\BSON\MaxKey)#%d (%d) {
}

object(MongoDB\BSON\MinKey)#%d (%d) {
}

object(MongoDB\BSON\ObjectId)#%d (%d) {
  ["oid"]=>
  string(24) "%x"
}
MongoDB\BSON\ObjectId::$oid exists: no

object(MongoDB\BSON\Regex)#%d (%d) {
  ["pattern"]=>
  string(3) "foo"
  ["flags"]=>
  string(1) "i"
}
MongoDB\BSON\Regex::$pattern exists: no
MongoDB\BSON\Regex::$flags exists: no

object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  string(4) "1234"
  ["timestamp"]=>
  string(4) "5678"
}
MongoDB\BSON\Timestamp::$increment exists: no
MongoDB\BSON\Timestamp::$timestamp exists: no

object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(%d) "%d"
}
MongoDB\BSON\UTCDateTime::$milliseconds exists: no

===DONE===
