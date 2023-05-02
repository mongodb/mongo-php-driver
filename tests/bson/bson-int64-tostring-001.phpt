--TEST--
MongoDB\BSON\Int64::__toString()
--FILE--
<?php

$tests = [
    '9223372036854775807',
    '-9223372036854775808',
    0,
];

foreach ($tests as $test) {
    var_dump((string) new MongoDB\BSON\Int64($test));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(19) "9223372036854775807"
string(20) "-9223372036854775808"
string(1) "0"
===DONE===
