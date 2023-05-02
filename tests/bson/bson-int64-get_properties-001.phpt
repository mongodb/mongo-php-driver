--TEST--
MongoDB\BSON\Int64 get_properties handler (get_object_vars)
--FILE--
<?php

$tests = [
    '9223372036854775807',
    '-9223372036854775808',
    0,
];

foreach ($tests as $test) {
    var_dump(get_object_vars(new MongoDB\BSON\Int64($test)));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(1) {
  ["integer"]=>
  string(19) "9223372036854775807"
}
array(1) {
  ["integer"]=>
  string(20) "-9223372036854775808"
}
array(1) {
  ["integer"]=>
  string(1) "0"
}
===DONE===
