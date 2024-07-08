--TEST--
MongoDB\Driver\ReadPreference::bsonSerialize() returns an object
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY_PREFERRED),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY_PREFERRED),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::NEAREST),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY, []),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'ny']]),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'ny'], ['dc' => 'sf', 'use' => 'reporting'], []]),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['maxStalenessSeconds' => 1000]),
];

foreach ($tests as $test) {
    var_dump($test->bsonSerialize());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["mode"]=>
  string(7) "primary"
}
object(stdClass)#%d (%d) {
  ["mode"]=>
  string(16) "primaryPreferred"
}
object(stdClass)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
}
object(stdClass)#%d (%d) {
  ["mode"]=>
  string(18) "secondaryPreferred"
}
object(stdClass)#%d (%d) {
  ["mode"]=>
  string(7) "nearest"
}
object(stdClass)#%d (%d) {
  ["mode"]=>
  string(7) "primary"
}
object(stdClass)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  array(1) {
    [0]=>
    object(stdClass)#%d (%d) {
      ["dc"]=>
      string(2) "ny"
    }
  }
}
object(stdClass)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  array(3) {
    [0]=>
    object(stdClass)#%d (%d) {
      ["dc"]=>
      string(2) "ny"
    }
    [1]=>
    object(stdClass)#%d (%d) {
      ["dc"]=>
      string(2) "sf"
      ["use"]=>
      string(9) "reporting"
    }
    [2]=>
    object(stdClass)#%d (%d) {
    }
  }
}
object(stdClass)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["maxStalenessSeconds"]=>
  int(1000)
}
===DONE===
