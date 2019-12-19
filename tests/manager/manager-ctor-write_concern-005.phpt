--TEST--
MongoDB\Driver\Manager::__construct(): write concern options (64-bit wtimeoutms)
--FILE--
<?php

$tests = [
    'mongodb://127.0.0.1/?w=2&wtimeoutms=4294967296',
    'mongodb://127.0.0.1/?w=majority&wtimeoutms=4294967296',
    'mongodb://127.0.0.1/?w=customTagSet&wtimeoutms=4294967296',
];

foreach ($tests as $uri) {
    $manager = new MongoDB\Driver\Manager($uri);
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
  %rint\(4294967296\)|string\(10\) "4294967296"%r
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(8) "majority"
  ["wtimeout"]=>
  %rint\(4294967296\)|string\(10\) "4294967296"%r
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(12) "customTagSet"
  ["wtimeout"]=>
  %rint\(4294967296\)|string\(10\) "4294967296"%r
}
===DONE===
