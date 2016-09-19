--TEST--
MongoDB\Driver\Manager::__construct(): read preference options (maxStalenessMS)
--FILE--
<?php

$tests = [
    ['mongodb://127.0.0.1/?readPreference=secondary&maxStalenessMS=1231', []],
    ['mongodb://127.0.0.1/?readPreference=secondary', ['maxStalenessMS' => 1231]],
    ['mongodb://127.0.0.1/?readPreference=secondary&maxStalenessMS=1000', ['maxStalenessMS' => 2000]],
    ['mongodb://127.0.0.1/?readpreference=secondary&maxstalenessms=1231', []],
    ['mongodb://127.0.0.1/?readpreference=secondary', ['maxstalenessms' => 1231]],
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
  ["maxStalenessMS"]=>
  int(1231)
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["maxStalenessMS"]=>
  int(1231)
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["maxStalenessMS"]=>
  int(2000)
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["maxStalenessMS"]=>
  int(1231)
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["maxStalenessMS"]=>
  int(1231)
}
===DONE===
