--TEST--
MongoDB\Driver\Manager::__construct(): write concern options (64-bit wtimeoutms)
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

$tests = [
    [null, ['w' => 2, 'wtimeoutms' => 4294967296]],
    [null, ['w' => 'majority', 'wtimeoutms' => 4294967296]],
    [null, ['w' => 'customTagSet', 'wtimeoutms' => 4294967296]],
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
  int(2)
  ["wtimeout"]=>
  int(4294967296)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(8) "majority"
  ["wtimeout"]=>
  int(4294967296)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(12) "customTagSet"
  ["wtimeout"]=>
  int(4294967296)
}
===DONE===
