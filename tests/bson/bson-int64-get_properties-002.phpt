--TEST--
MongoDB\BSON\Int64 get_properties handler (foreach)
--FILE--
<?php

$tests = [
    '9223372036854775807',
    '-9223372036854775808',
    0,
];

foreach ($tests as $test) {
    $int64 = new MongoDB\BSON\Int64($test);
    foreach ($int64 as $key => $value) {
        var_dump($key);
        var_dump($value);
    }
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(7) "integer"
string(19) "9223372036854775807"
string(7) "integer"
string(20) "-9223372036854775808"
string(7) "integer"
string(1) "0"
===DONE===
