--TEST--
MongoDB\Driver\Manager::__construct(): write concern options (safe)
--FILE--
<?php

$tests = [
    ['mongodb://127.0.0.1/?safe=true', []],
    ['mongodb://127.0.0.1/?safe=false', []],
    // w overrides safe if provided in the same format
    ['mongodb://127.0.0.1/?w=1&safe=false', []],
    ['mongodb://127.0.0.1/?w=0&safe=true', []],
    [null, ['safe' => true]],
    [null, ['safe' => false]],
    [null, ['w' => 1, 'safe' => false]],
    [null, ['w' => 0, 'safe' => true]],
    // safe in URI options array may override w in URI string
    ['mongodb://127.0.0.1/?w=0', ['safe' => true]],
    ['mongodb://127.0.0.1/?w=1', ['safe' => false]],
];

foreach ($tests as $test) {
    list($uri, $options) = $test;

    $manager = new MongoDB\Driver\Manager($uri, $options);
    var_dump($manager->getWriteConcern());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(1)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(0)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(1)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(0)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(1)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(0)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(1)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(0)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(1)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(0)
}
===DONE===
