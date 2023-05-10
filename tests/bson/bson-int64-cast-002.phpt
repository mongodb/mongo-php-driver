--TEST--
MongoDB\BSON\Int64 casting (64-bit platform)
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

$tests = [
    new MongoDB\BSON\Int64(9223372036854775807),
    new MongoDB\BSON\Int64(~9223372036854775807),
];

foreach ($tests as $test) {
    var_dump($test);
    var_dump((int) $test);
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
string(19) "9223372036854775807"
bool(true)
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(20) "-9223372036854775808"
}
int(-9223372036854775808)
string(20) "-9223372036854775808"
bool(true)
===DONE===
