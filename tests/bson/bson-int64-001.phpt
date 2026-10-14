--TEST--
MongoDB\BSON\Int64 roundtripped through BSON on 32-bit platforms
--SKIPIF--
<?php if (4 !== PHP_INT_SIZE) { die('skip Only for 32-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* Omit testing values in the range of 32-bit integers. They will come back from
 * BSON as a PHP integer type, as Int64 is only used when necessary. */
$tests = [
    (object) ['int64' => new MongoDB\BSON\Int64('9223372036854775807')],
    (object) ['int64' => new MongoDB\BSON\Int64('-9223372036854775808')],
    (object) ['int64' => new MongoDB\BSON\Int64('2147483648')],
    (object) ['int64' => new MongoDB\BSON\Int64('-2147483649')],
];

foreach($tests as $test) {
    $bson = MongoDB\BSON\Document::fromPHP($test);
    $testRoundtripped = $bson->toPHP();
    $bsonRoundtripped = MongoDB\BSON\Document::fromPHP($testRoundtripped);
    $json = $bson->toRelaxedExtendedJSON();
    $jsonRoundtripped = $bsonRoundtripped->toRelaxedExtendedJSON();

    var_dump($test->int64 instanceof MongoDB\BSON\Int64);
    var_dump($testRoundtripped->int64 instanceof MongoDB\BSON\Int64);
    var_dump($json, $jsonRoundtripped);
    var_dump($test == $testRoundtripped);

    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(true)
string(33) "{ "int64" : 9223372036854775807 }"
string(33) "{ "int64" : 9223372036854775807 }"
bool(true)

bool(true)
bool(true)
string(34) "{ "int64" : -9223372036854775808 }"
string(34) "{ "int64" : -9223372036854775808 }"
bool(true)

bool(true)
bool(true)
string(24) "{ "int64" : 2147483648 }"
string(24) "{ "int64" : 2147483648 }"
bool(true)

bool(true)
bool(true)
string(25) "{ "int64" : -2147483649 }"
string(25) "{ "int64" : -2147483649 }"
bool(true)

===DONE===
