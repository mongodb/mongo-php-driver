--TEST--
MongoDB\Driver\Manager::__construct(): read preference options of the wrong type will not be ignored
--FILE--
<?php

$tests = [
    ['mongodb://127.0.0.1/?readPreference=secondary', ['readPreference' => 1]],
    ['mongodb://127.0.0.1/?readPreference=secondary&readPreferenceTags=tag:one', ['readPreferenceTags' => 'invalid']],
    ['mongodb://127.0.0.1/?readPreference=secondary&maxStalenessSeconds=1000', ['maxStalenessSeconds' => 'invalid']],
];

foreach ($tests as $test) {
    list($uri, $options) = $test;

    try {
        $manager = new MongoDB\Driver\Manager($uri, $options);
    } catch (MongoDB\Driver\Exception\InvalidArgumentException $e) {
        echo $e->getMessage() . "\n";
        continue;
    }
    var_dump($manager->getReadPreference());
}

?>
===DONE===
--EXPECTF--
readPreference URI option expected to be string
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  array(1) {
    [0]=>
    object(stdClass)#%d (%d) {
      ["tag"]=>
      string(3) "one"
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
