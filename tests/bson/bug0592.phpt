--TEST--
PHPC-592: Property name corrupted when unserializing 64-bit integer on 32-bit platform
--SKIPIF--
<?php if (4 !== PHP_INT_SIZE) { die('skip Only for 32-bit platform'); } ?>
--INI--
mongodb.debug=stderr
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    '{ "x": { "$numberLong": "-2147483648" }}',
    '{ "x": { "$numberLong": "2147483647" }}',
    '{ "x": { "$numberLong": "4294967294" }}',
    '{ "x": { "$numberLong": "4294967295" }}',
    '{ "x": { "$numberLong": "9223372036854775807" }}',
    '{ "longFieldName": { "$numberLong": "-2147483648" }}',
    '{ "longFieldName": { "$numberLong": "2147483647" }}',
    '{ "longFieldName": { "$numberLong": "4294967294" }}',
    '{ "longFieldName": { "$numberLong": "4294967295" }}',
    '{ "longFieldName": { "$numberLong": "9223372036854775807" }}',
];

foreach ($tests as $json) {
    printf("Test %s\n", $json);
    var_dump(toPHP(fromJSON($json)));
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%a
Test { "x": { "$numberLong": "-2147483648" }}
object(stdClass)#%d (%d) {
  ["x"]=>
  int(-2147483648)
}

Test { "x": { "$numberLong": "2147483647" }}
object(stdClass)#%d (%d) {
  ["x"]=>
  int(2147483647)
}

Test { "x": { "$numberLong": "4294967294" }}
[%s] PHONGO-BSON: WARNING > Integer overflow detected on your platform: 4294967294
object(stdClass)#%d (%d) {
  ["x"]=>
  string(10) "4294967294"
}

Test { "x": { "$numberLong": "4294967295" }}
[%s] PHONGO-BSON: WARNING > Integer overflow detected on your platform: 4294967295
object(stdClass)#%d (%d) {
  ["x"]=>
  string(10) "4294967295"
}

Test { "x": { "$numberLong": "9223372036854775807" }}
[%s] PHONGO-BSON: WARNING > Integer overflow detected on your platform: 9223372036854775807
object(stdClass)#%d (%d) {
  ["x"]=>
  string(19) "9223372036854775807"
}

Test { "longFieldName": { "$numberLong": "-2147483648" }}
object(stdClass)#%d (%d) {
  ["longFieldName"]=>
  int(-2147483648)
}

Test { "longFieldName": { "$numberLong": "2147483647" }}
object(stdClass)#%d (%d) {
  ["longFieldName"]=>
  int(2147483647)
}

Test { "longFieldName": { "$numberLong": "4294967294" }}
[%s] PHONGO-BSON: WARNING > Integer overflow detected on your platform: 4294967294
object(stdClass)#%d (%d) {
  ["longFieldName"]=>
  string(10) "4294967294"
}

Test { "longFieldName": { "$numberLong": "4294967295" }}
[%s] PHONGO-BSON: WARNING > Integer overflow detected on your platform: 4294967295
object(stdClass)#%d (%d) {
  ["longFieldName"]=>
  string(10) "4294967295"
}

Test { "longFieldName": { "$numberLong": "9223372036854775807" }}
[%s] PHONGO-BSON: WARNING > Integer overflow detected on your platform: 9223372036854775807
object(stdClass)#%d (%d) {
  ["longFieldName"]=>
  string(19) "9223372036854775807"
}

===DONE===
