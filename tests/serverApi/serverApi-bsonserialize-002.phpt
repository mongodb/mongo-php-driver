--TEST--
MongoDB\Driver\ServerApi::bsonSerialize() returns an object
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    new MongoDB\Driver\ServerApi('1'),
    new MongoDB\Driver\ServerApi('1', true),
    new MongoDB\Driver\ServerApi('1', null, true),
    new MongoDB\Driver\ServerApi('1', false, false),
];

foreach ($tests as $test) {
    var_dump($test->bsonSerialize());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["version"]=>
  string(1) "1"
}
object(stdClass)#%d (%d) {
  ["version"]=>
  string(1) "1"
  ["strict"]=>
  bool(true)
}
object(stdClass)#%d (%d) {
  ["version"]=>
  string(1) "1"
  ["deprecationErrors"]=>
  bool(true)
}
object(stdClass)#%d (%d) {
  ["version"]=>
  string(1) "1"
  ["strict"]=>
  bool(false)
  ["deprecationErrors"]=>
  bool(false)
}
===DONE===
