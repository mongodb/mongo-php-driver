--TEST--
MongoDB\Driver\Manager::__construct(): read preference options
--FILE--
<?php

$tests = [
    ['mongodb://127.0.0.1/?readPreference=primary', []],
    ['mongodb://127.0.0.1/?readPreference=secondary&readPreferenceTags=tag:one&readPreferenceTags=', []],
    ['mongodb://127.0.0.1/?readPreference=secondary&maxStalenessSeconds=1000', []],
    [null, ['readPreference' => 'primary']],
    [null, ['readPreference' => 'secondary', 'readPreferenceTags' => [['tag' => 'one'], []]]],
    [null, ['readPreference' => 'secondary', 'maxStalenessSeconds' => 1000]],
];

foreach ($tests as $test) {
    list($uri, $options) = $test;

    $manager = new MongoDB\Driver\Manager($uri, $options);
    var_dump($manager->getReadPreference());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "primary"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  array(2) {
    [0]=>
    object(stdClass)#%d (%d) {
      ["tag"]=>
      string(3) "one"
    }
    [1]=>
    object(stdClass)#%d (%d) {
    }
  }
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["maxStalenessSeconds"]=>
  int(1000)
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "primary"
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  array(2) {
    [0]=>
    object(stdClass)#%d (%d) {
      ["tag"]=>
      string(3) "one"
    }
    [1]=>
    object(stdClass)#%d (%d) {
    }
  }
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["maxStalenessSeconds"]=>
  int(1000)
}
===DONE===
