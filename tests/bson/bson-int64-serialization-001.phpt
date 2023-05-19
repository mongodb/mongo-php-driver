--TEST--
MongoDB\BSON\Int64 serialization (Serializable interface)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '7.4.0'); ?>
--FILE--
<?php

$tests = [
    new MongoDB\BSON\Int64('9223372036854775807'),
    new MongoDB\BSON\Int64('-9223372036854775808'),
    new MongoDB\BSON\Int64(0),
];

foreach ($tests as $test) {
    var_dump($test);
    var_dump($s = serialize($test));
    var_dump(unserialize($s));
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(19) "9223372036854775807"
}
string(78) "C:18:"MongoDB\BSON\Int64":47:{a:1:{s:7:"integer";s:19:"9223372036854775807";}}"
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(19) "9223372036854775807"
}

object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(20) "-9223372036854775808"
}
string(79) "C:18:"MongoDB\BSON\Int64":48:{a:1:{s:7:"integer";s:20:"-9223372036854775808";}}"
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(20) "-9223372036854775808"
}

object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(1) "0"
}
string(59) "C:18:"MongoDB\BSON\Int64":28:{a:1:{s:7:"integer";s:1:"0";}}"
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(1) "0"
}

===DONE===
