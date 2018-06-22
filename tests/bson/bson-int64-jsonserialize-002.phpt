--TEST--
MongoDB\BSON\Int64::jsonSerialize() with json_encode()
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    ['max' => unserialize('C:18:"MongoDB\BSON\Int64":47:{a:1:{s:7:"integer";s:19:"9223372036854775807";}}')],
    ['min' => unserialize('C:18:"MongoDB\BSON\Int64":48:{a:1:{s:7:"integer";s:20:"-9223372036854775808";}}')],
    ['zero' => unserialize('C:18:"MongoDB\BSON\Int64":28:{a:1:{s:7:"integer";s:1:"0";}}')],
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
