--TEST--
MongoDB\Driver\Manager::__construct(): read preference options
--FILE--
<?php

$tests = [
    ['mongodb://127.0.0.1/?readPreference=primary', []],
    ['mongodb://127.0.0.1/?readPreference=secondary&readPreferenceTags=tag:one&readPreferenceTags=', []],
    ['mongodb://127.0.0.1/?readPreference=secondary&maxStalenessMS=1000', []],
    [null, ['readPreference' => 'primary']],
    [null, ['readPreference' => 'secondary', 'readPreferenceTags' => [['tag' => 'one'], []]]],
    [null, ['readPreference' => 'secondary', 'maxStalenessMS' => 1000]],
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
    array(1) {
      ["tag"]=>
      string(3) "one"
    }
    [1]=>
    array(0) {
    }
  }
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["maxStalenessMS"]=>
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
    array(1) {
      ["tag"]=>
      string(3) "one"
    }
    [1]=>
    array(0) {
    }
  }
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["maxStalenessMS"]=>
  int(1000)
}
===DONE===
