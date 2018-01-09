--TEST--
MongoDB\BSON\Javascript get_properties handler (get_object_vars)
--FILE--
<?php

$tests = [
    new MongoDB\BSON\Javascript('function foo(bar) { return bar; }'),
    new MongoDB\BSON\Javascript('function foo() { return bar; }', ['bar' => 42]),
];

foreach ($tests as $test) {
    var_dump(get_object_vars($test));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(2) {
  ["code"]=>
  string(33) "function foo(bar) { return bar; }"
  ["scope"]=>
  NULL
}
array(2) {
  ["code"]=>
  string(30) "function foo() { return bar; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
    ["bar"]=>
    int(42)
  }
}
===DONE===
