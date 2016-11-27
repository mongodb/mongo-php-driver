--TEST--
MongoDB\Driver\Manager::__construct(): read preference options (maxStalenessSeconds)
--FILE--
<?php

$tests = [
    ['mongodb://127.0.0.1/?readPreference=secondary&maxStalenessSeconds=1231', []],
    ['mongodb://127.0.0.1/?readPreference=secondary', ['maxStalenessSeconds' => 1231]],
    ['mongodb://127.0.0.1/?readPreference=secondary&maxStalenessSeconds=1000', ['maxStalenessSeconds' => 2000]],
    ['mongodb://127.0.0.1/?readpreference=secondary&maxstalenessseconds=1231', []],
    ['mongodb://127.0.0.1/?readpreference=secondary', ['maxstalenessseconds' => 1231]],
];

foreach ($tests as $test) {
    list($uri, $options) = $test;

    $manager = new MongoDB\Driver\Manager($uri, $options);
    var_dump($manager->getReadPreference());
}

?>
===DONE===
--EXPECTF--
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["maxStalenessSeconds"]=>
  int(1231)
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["maxStalenessSeconds"]=>
  int(1231)
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["maxStalenessSeconds"]=>
  int(2000)
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["maxStalenessSeconds"]=>
  int(1231)
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["maxStalenessSeconds"]=>
  int(1231)
}
===DONE===
