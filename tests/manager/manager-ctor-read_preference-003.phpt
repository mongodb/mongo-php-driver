--TEST--
MongoDB\Driver\Manager::__construct(): read preference options of the wrong type are ignored
--FILE--
<?php

$tests = [
    ['mongodb://127.0.0.1/?readPreference=secondary', ['readPreference' => 1]],
    ['mongodb://127.0.0.1/?readPreference=secondary&readPreferenceTags=tag:one', ['readPreferenceTags' => 'invalid']],
    ['mongodb://127.0.0.1/?readPreference=secondary&maxStalenessMS=1000', ['maxStalenessMS' => 'invalid']],
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
  ["tags"]=>
  array(1) {
    [0]=>
    array(1) {
      ["tag"]=>
      string(3) "one"
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
