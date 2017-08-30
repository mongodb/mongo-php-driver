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
        var_dump($object->{$property});
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

Notice: Undefined property: MongoDB\BSON\Binary::$data in %s on line %d
NULL

Notice: Undefined property: MongoDB\BSON\Binary::$type in %s on line %d
NULL

object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(4) "3.14"
}

Notice: Undefined property: MongoDB\BSON\Decimal128::$dec in %s on line %d
NULL

object(MongoDB\BSON\Javascript)#%d (%d) {
  ["code"]=>
  string(30) "function foo() { return bar; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
    ["bar"]=>
    int(42)
  }
}

Notice: Undefined property: MongoDB\BSON\Javascript::$code in %s on line %d
NULL

Notice: Undefined property: MongoDB\BSON\Javascript::$scope in %s on line %d
NULL

object(MongoDB\BSON\MaxKey)#%d (%d) {
}

object(MongoDB\BSON\MinKey)#%d (%d) {
}

object(MongoDB\BSON\ObjectId)#%d (%d) {
  ["oid"]=>
  string(24) "%x"
}

Notice: Undefined property: MongoDB\BSON\ObjectId::$oid in %s on line %d
NULL

object(MongoDB\BSON\Regex)#%d (%d) {
  ["pattern"]=>
  string(3) "foo"
  ["flags"]=>
  string(1) "i"
}

Notice: Undefined property: MongoDB\BSON\Regex::$pattern in %s on line %d
NULL

Notice: Undefined property: MongoDB\BSON\Regex::$flags in %s on line %d
NULL

object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  string(4) "1234"
  ["timestamp"]=>
  string(4) "5678"
}

Notice: Undefined property: MongoDB\BSON\Timestamp::$increment in %s on line %d
NULL

Notice: Undefined property: MongoDB\BSON\Timestamp::$timestamp in %s on line %d
NULL

object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(%d) "%d"
}

Notice: Undefined property: MongoDB\BSON\UTCDateTime::$milliseconds in %s on line %d
NULL

===DONE===
