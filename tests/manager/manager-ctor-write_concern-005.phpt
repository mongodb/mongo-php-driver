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
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  int(2)
  ["j"]=>
  NULL
  ["wtimeout"]=>
  %rint\(4294967296\)|int\(4294967295\)%r
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  string(8) "majority"
  ["j"]=>
  NULL
  ["wtimeout"]=>
  %rint\(4294967296\)|int\(4294967295\)%r
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  string(12) "customTagSet"
  ["j"]=>
  NULL
  ["wtimeout"]=>
  %rint\(4294967296\)|int\(4294967295\)%r
}
===DONE===
