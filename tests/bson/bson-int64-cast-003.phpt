--TEST--
MongoDB\BSON\Int64 casting (32-bit platform)
--SKIPIF--
<?php if (4 !== PHP_INT_SIZE) { die('skip Only for 32-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    new MongoDB\BSON\Int64('9223372036854775807'),
    new MongoDB\BSON\Int64('-9223372036854775808'),
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

Warning: Truncating 64-bit integer value 9223372036854775807 to 32 bits %s
int(-1)
float(9.22%d%r[eE]%r+18)
string(19) "9223372036854775807"
bool(true)
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(20) "-9223372036854775808"
}

Warning: Truncating 64-bit integer value -9223372036854775808 to 32 bits %s
int(0)
float(-9.22%d%r[eE]%r+18)
string(20) "-9223372036854775808"
bool(true)
===DONE===
