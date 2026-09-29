--TEST--
MongoDB\Driver\ReadPreference construction
--FILE--
<?php

var_dump(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY));
var_dump(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['tag' => 'one']]));
var_dump(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY, []));
var_dump(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['maxStalenessSeconds' => 1000]));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "primary"
  ["tags"]=>
  NULL
  ["maxStalenessSeconds"]=>
  int(-1)
  ["hedge"]=>
  NULL
}
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
  ["maxStalenessSeconds"]=>
  int(-1)
  ["hedge"]=>
  NULL
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "primary"
  ["tags"]=>
  NULL
  ["maxStalenessSeconds"]=>
  int(-1)
  ["hedge"]=>
  NULL
}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  NULL
  ["maxStalenessSeconds"]=>
  int(1000)
  ["hedge"]=>
  NULL
}
===DONE===
