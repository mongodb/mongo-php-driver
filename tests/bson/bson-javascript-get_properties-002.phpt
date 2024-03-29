--TEST--
MongoDB\BSON\Javascript get_properties handler (foreach)
--FILE--
<?php

$tests = [
    new MongoDB\BSON\Javascript('function(bar) { return bar; }'),
    new MongoDB\BSON\Javascript('function() { return bar; }', ['bar' => 42]),
];

foreach ($tests as $test) {
    foreach ($test as $key => $value) {
        var_dump($key);
        var_dump($value);
    }
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
string(4) "code"
string(29) "function(bar) { return bar; }"
string(5) "scope"
NULL
string(4) "code"
string(26) "function() { return bar; }"
string(5) "scope"
object(stdClass)#%d (%d) {
  ["bar"]=>
  int(42)
}
===DONE===
