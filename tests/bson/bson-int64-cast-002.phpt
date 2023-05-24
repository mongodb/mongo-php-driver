--TEST--
MongoDB\BSON\Int64 casting (64-bit platform)
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

$tests = [
    new MongoDB\BSON\Int64(PHP_INT_MAX),
    new MongoDB\BSON\Int64(PHP_INT_MIN),
];

foreach ($tests as $test) {
    var_dump($test);
    var_dump((int) $test);
    var_dump((float) $test);
    var_dump((string) $test);
    var_dump((bool) $test);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(19) "9223372036854775807"
}
int(9223372036854775807)
float(9.22%d%r[eE]%r+18)
string(19) "9223372036854775807"
bool(true)
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(20) "-9223372036854775808"
}
int(-9223372036854775808)
float(-9.22%d%r[eE]%r+18)
string(20) "-9223372036854775808"
bool(true)
===DONE===
