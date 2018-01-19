--TEST--
PHPC-341: fromJSON() leaks when JSON contains array or object fields
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = array(
    '{ "foo": "yes", "bar" : false }',
    '{ "foo": "no", "array" : [ 5, 6 ] }',
    '{ "foo": "no", "obj" : { "embedded" : 4.125 } }',
);

foreach ($tests as $test) {
    $bson = fromJSON($test);
    var_dump(toPHP($bson));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (2) {
  ["foo"]=>
  string(3) "yes"
  ["bar"]=>
  bool(false)
}
object(stdClass)#%d (2) {
  ["foo"]=>
  string(2) "no"
  ["array"]=>
  array(2) {
    [0]=>
    int(5)
    [1]=>
    int(6)
  }
}
object(stdClass)#%d (2) {
  ["foo"]=>
  string(2) "no"
  ["obj"]=>
  object(stdClass)#%d (1) {
    ["embedded"]=>
    float(4.125)
  }
}
===DONE===
