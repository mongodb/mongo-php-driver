--TEST--
MongoDB\Driver\Manager::__construct(): read concern options
--FILE--
<?php

$tests = [
    ['mongodb://127.0.0.1/?readConcernLevel=local', []],
    // libmongoc does not discriminate numeric strings for expected string types
    ['mongodb://127.0.0.1/?readConcernLevel=1', []],
    [null, ['readConcernLevel' => 'local']],
];

foreach ($tests as $test) {
    list($uri, $options) = $test;

    $manager = new MongoDB\Driver\Manager($uri, $options);
    var_dump($manager->getReadConcern());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(5) "local"
}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(1) "1"
}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(5) "local"
}
===DONE===
