--TEST--
MongoDB\BSON\Int64::jsonSerialize() with json_encode()
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    ['max' => new MongoDB\BSON\Int64('9223372036854775807')],
    ['min' => new MongoDB\BSON\Int64('-9223372036854775808')],
    ['zero' => new MongoDB\BSON\Int64(0)],
];

foreach ($tests as $test) {
    var_dump(json_encode($test));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(45) "{"max":{"$numberLong":"9223372036854775807"}}"
string(46) "{"min":{"$numberLong":"-9223372036854775808"}}"
string(28) "{"zero":{"$numberLong":"0"}}"
===DONE===
