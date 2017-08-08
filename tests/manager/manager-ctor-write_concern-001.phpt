--TEST--
MongoDB\Driver\Manager::__construct(): write concern options (w)
--FILE--
<?php

$tests = [
    ['mongodb://127.0.0.1/?w=-1', []],
    ['mongodb://127.0.0.1/?w=0', []],
    ['mongodb://127.0.0.1/?w=1', []],
    ['mongodb://127.0.0.1/?w=majority', []],
    ['mongodb://127.0.0.1/?w=customTagSet', []],
    [null, ['w' => -1]],
    [null, ['w' => -0]],
    [null, ['w' => 1]],
    [null, ['w' => 'majority']],
    [null, ['w' => 'customTagSet']],
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
  int(-1)
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
  string(8) "majority"
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(12) "customTagSet"
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(-1)
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
  string(8) "majority"
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(12) "customTagSet"
}
===DONE===
