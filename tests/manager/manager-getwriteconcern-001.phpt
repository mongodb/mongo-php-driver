--TEST--
MongoDB\Driver\Manager::getWriteConcern()
--FILE--
<?php

$tests = array(
    array(null, array()),
    array('mongodb://127.0.0.1/?w=1', array()),
    array('mongodb://127.0.0.1/?w=majority', array()),
    array(null, array('w' => 1, 'journal' => true)),
    array(null, array('w' => 'majority', 'journal' => true)),
    array('mongodb://127.0.0.1/?w=majority&journal=true', array('w' => 1, 'journal' => false)),
    array('mongodb://127.0.0.1/?wtimeoutms=1000', array()),
    array(null, array('wtimeoutms' => 1000)),
    array('mongodb://127.0.0.1/?w=2', array('wtimeoutms' => 1000)),
    array('mongodb://127.0.0.1/?w=majority', array('wtimeoutms' => 1000)),
    array('mongodb://127.0.0.1/?w=customTagSet', array('wtimeoutms' => 1000)),
);

foreach ($tests as $i => $test) {
    list($uri, $options) = $test;

    $manager = new MongoDB\Driver\Manager($uri, $options);
    var_dump($manager->getWriteConcern());

    $manager->getWriteConcern();
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  NULL
  ["j"]=>
  NULL
  ["wtimeout"]=>
  int(0)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  int(1)
  ["j"]=>
  NULL
  ["wtimeout"]=>
  int(0)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  string(8) "majority"
  ["j"]=>
  NULL
  ["wtimeout"]=>
  int(0)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  int(1)
  ["j"]=>
  bool(true)
  ["wtimeout"]=>
  int(0)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  string(8) "majority"
  ["j"]=>
  bool(true)
  ["wtimeout"]=>
  int(0)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  int(1)
  ["j"]=>
  bool(false)
  ["wtimeout"]=>
  int(0)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  NULL
  ["j"]=>
  NULL
  ["wtimeout"]=>
  int(1000)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  NULL
  ["j"]=>
  NULL
  ["wtimeout"]=>
  int(1000)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  int(2)
  ["j"]=>
  NULL
  ["wtimeout"]=>
  int(1000)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  string(8) "majority"
  ["j"]=>
  NULL
  ["wtimeout"]=>
  int(1000)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  string(12) "customTagSet"
  ["j"]=>
  NULL
  ["wtimeout"]=>
  int(1000)
}
===DONE===
