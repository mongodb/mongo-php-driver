--TEST--
MongoDB\Driver\Manager::getReadConcern()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$tests = [
    [STANDALONE, []],
    [STANDALONE . '/?readconcernlevel=local', []],
    [STANDALONE . '/?readconcernlevel=majority', []],
    [STANDALONE . '/?readconcernlevel=not-yet-supported', []],
    [STANDALONE, ['readconcernlevel' => 'local']],
    [STANDALONE, ['readconcernlevel' => 'majority']],
    [STANDALONE, ['readconcernlevel' => 'not-yet-supported']],
    [STANDALONE . '/?readconcernlevel=local', ['readconcernlevel' => 'majority']],
];

foreach ($tests as $i => $test) {
    list($uri, $options) = $test;

    $manager = new MongoDB\Driver\Manager($uri, $options);
    var_dump($manager->getReadConcern());

    // Test for !return_value_used
    $manager->getReadConcern();
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\ReadConcern)#%d (%d) {
}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(5) "local"
}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(8) "majority"
}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(17) "not-yet-supported"
}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(5) "local"
}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(8) "majority"
}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(17) "not-yet-supported"
}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(8) "majority"
}
===DONE===
