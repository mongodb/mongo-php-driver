--TEST--
MongoDB\Driver\ReadConcern::bsonSerialize() returns an object
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    new MongoDB\Driver\ReadConcern(),
    new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LOCAL),
];

foreach ($tests as $test) {
    var_dump($test->bsonSerialize());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
}
object(stdClass)#%d (%d) {
  ["level"]=>
  string(5) "local"
}
===DONE===
