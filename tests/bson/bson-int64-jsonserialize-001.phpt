--TEST--
MongoDB\BSON\Int64::jsonSerialize() return value
--FILE--
<?php

$tests = [
    '9223372036854775807',
    '-9223372036854775808',
    0,
];

foreach ($tests as $test) {
    $int64 = new MongoDB\BSON\Int64($test);
    var_dump($int64->jsonSerialize());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(1) {
  ["$numberLong"]=>
  string(19) "9223372036854775807"
}
array(1) {
  ["$numberLong"]=>
  string(20) "-9223372036854775808"
}
array(1) {
  ["$numberLong"]=>
  string(1) "0"
}
===DONE===
