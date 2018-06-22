--TEST--
MongoDB\BSON\Int64::__toString()
--FILE--
<?php

$tests = [
    unserialize('C:18:"MongoDB\BSON\Int64":47:{a:1:{s:7:"integer";s:19:"9223372036854775807";}}'),
    unserialize('C:18:"MongoDB\BSON\Int64":48:{a:1:{s:7:"integer";s:20:"-9223372036854775808";}}'),
    unserialize('C:18:"MongoDB\BSON\Int64":28:{a:1:{s:7:"integer";s:1:"0";}}'),
];

foreach ($tests as $test) {
    var_dump((string) $test);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(19) "9223372036854775807"
string(20) "-9223372036854775808"
string(1) "0"
===DONE===
