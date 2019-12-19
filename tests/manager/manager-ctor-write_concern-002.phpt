--TEST--
MongoDB\Driver\Manager::__construct(): write concern options (wtimeoutms)
--FILE--
<?php

$tests = [
    ['mongodb://127.0.0.1/?wtimeoutms=1000', []],
    // 64-bit wtimeout may be reported as integer or string
    ['mongodb://127.0.0.1/?wtimeoutms=4294967296', []],
    ['mongodb://127.0.0.1/?w=2&wtimeoutms=1000', []],
    ['mongodb://127.0.0.1/?w=majority&wtimeoutms=1000', []],
    ['mongodb://127.0.0.1/?w=customTagSet&wtimeoutms=1000', []],
    [null, ['wtimeoutms' => 1000]],
    [null, ['w' => 2, 'wtimeoutms' => 1000]],
    [null, ['w' => 'majority', 'wtimeoutms' => 1000]],
    [null, ['w' => 'customTagSet', 'wtimeoutms' => 1000]],
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
  ["wtimeout"]=>
  int(1000)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["wtimeout"]=>
  %rint\(4294967296\)|string\(10\) "4294967296"%r
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(2)
  ["wtimeout"]=>
  int(1000)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(8) "majority"
  ["wtimeout"]=>
  int(1000)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(12) "customTagSet"
  ["wtimeout"]=>
  int(1000)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["wtimeout"]=>
  int(1000)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(2)
  ["wtimeout"]=>
  int(1000)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(8) "majority"
  ["wtimeout"]=>
  int(1000)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(12) "customTagSet"
  ["wtimeout"]=>
  int(1000)
}
===DONE===
