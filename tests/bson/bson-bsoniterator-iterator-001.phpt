--TEST--
MongoDB\BSON\BSONIterator iterating objects and arrays
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    '{}',
    '{ "foo": "bar" }',
    '{ "foo": "bar", "bar": "foo" }',
    '[]',
    '["foo", "bar"]',
];

foreach ($tests as $test) {
    printf("Test %s\n", $test);
    $iterator = MongoDB\BSON\Document::fromJSON($test)->getIterator();
    var_dump(iterator_to_array($iterator));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Test {}
array(0) {
}
Test { "foo": "bar" }
array(1) {
  ["foo"]=>
  string(3) "bar"
}
Test { "foo": "bar", "bar": "foo" }
array(2) {
  ["foo"]=>
  string(3) "bar"
  ["bar"]=>
  string(3) "foo"
}
Test []
array(0) {
}
Test ["foo", "bar"]
array(2) {
  [0]=>
  string(3) "foo"
  [1]=>
  string(3) "bar"
}
===DONE===
