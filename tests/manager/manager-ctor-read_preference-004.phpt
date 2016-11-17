--TEST--
MongoDB\Driver\Manager::__construct(): read preference options (maxStalenessSeconds default)
--FILE--
<?php

$tests = [
    ['mongodb://127.0.0.1/?readPreference=secondary&maxStalenessSeconds=-1', []],
    ['mongodb://127.0.0.1/?readPreference=secondary&maxStalenessSeconds=-1.0', []],
    ['mongodb://127.0.0.1/?readPreference=secondary', ['maxStalenessSeconds' => -1]],
    ['mongodb://127.0.0.1/?readPreference=secondary', ['maxStalenessSeconds' => -1.0]],
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
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
}
===DONE===
