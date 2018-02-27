--TEST--
MongoDB\Driver\Manager::getReadConcern()
--FILE--
<?php

$tests = [
    [null, []],
    ['mongodb://127.0.0.1/?readconcernlevel=local', []],
    ['mongodb://127.0.0.1/?readconcernlevel=majority', []],
    ['mongodb://127.0.0.1/?readconcernlevel=not-yet-supported', []],
    [null, ['readconcernlevel' => 'local']],
    [null, ['readconcernlevel' => 'majority']],
    [null, ['readconcernlevel' => 'not-yet-supported']],
    ['mongodb://127.0.0.1/?readconcernlevel=local', ['readconcernlevel' => 'majority']],
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
