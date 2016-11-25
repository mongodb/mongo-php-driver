--TEST--
MongoDB\Driver\Manager::__construct(): read preference options (maxStalenessSeconds)
--FILE--
<?php

$tests = [
    ['mongodb://127.0.0.1/?readPreference=secondary&maxStalenessSeconds=1.231', []],
    ['mongodb://127.0.0.1/?readPreference=secondary', ['maxStalenessSeconds' => 1.231]],
    ['mongodb://127.0.0.1/?readPreference=secondary&maxStalenessSeconds=1.000', ['maxStalenessSeconds' => 2.000]],
    ['mongodb://127.0.0.1/?readpreference=secondary&maxStalenessSeconds=1.231', []],
    ['mongodb://127.0.0.1/?readpreference=secondary', ['maxStalenessSeconds' => 1.231]],
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
  float(1.231)
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["maxStalenessSeconds"]=>
  float(1.231)
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["maxStalenessSeconds"]=>
  float(2)
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["maxStalenessSeconds"]=>
  float(1.231)
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["maxStalenessSeconds"]=>
  float(1.231)
}
===DONE===
