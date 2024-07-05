--TEST--
MongoDB\Driver\ReadPreference construction with integer constants (PHP 8.3+)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.3.0'); ?>
--FILE--
<?php

var_dump(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY));
var_dump(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, [['tag' => 'one']]));
var_dump(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY, []));
var_dump(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, null, ['maxStalenessSeconds' => 1000]));
var_dump(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, null, ['hedge' => ['enabled' => true]]));
var_dump(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, null, ['hedge' => []]));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: Constant MongoDB\Driver\ReadPreference::RP_PRIMARY is deprecated in %s

Deprecated: MongoDB\Driver\ReadPreference::__construct(): Passing an integer mode to "MongoDB\Driver\ReadPreference::__construct" is deprecated and will be removed in a future release. in %s
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "primary"
}

Deprecated: Constant MongoDB\Driver\ReadPreference::RP_SECONDARY is deprecated in %s

Deprecated: MongoDB\Driver\ReadPreference::__construct(): Passing an integer mode to "MongoDB\Driver\ReadPreference::__construct" is deprecated and will be removed in a future release. in %s
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

Deprecated: Constant MongoDB\Driver\ReadPreference::RP_PRIMARY is deprecated in %s

Deprecated: MongoDB\Driver\ReadPreference::__construct(): Passing an integer mode to "MongoDB\Driver\ReadPreference::__construct" is deprecated and will be removed in a future release. in %s
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "primary"
}

Deprecated: Constant MongoDB\Driver\ReadPreference::RP_SECONDARY is deprecated in %s

Deprecated: MongoDB\Driver\ReadPreference::__construct(): Passing an integer mode to "MongoDB\Driver\ReadPreference::__construct" is deprecated and will be removed in a future release. in %s
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["maxStalenessSeconds"]=>
  int(1000)
}

Deprecated: Constant MongoDB\Driver\ReadPreference::RP_SECONDARY is deprecated in %s

Deprecated: MongoDB\Driver\ReadPreference::__construct(): Passing an integer mode to "MongoDB\Driver\ReadPreference::__construct" is deprecated and will be removed in a future release. in %s
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["hedge"]=>
  object(stdClass)#%d (%d) {
    ["enabled"]=>
    bool(true)
  }
}

Deprecated: Constant MongoDB\Driver\ReadPreference::RP_SECONDARY is deprecated in %s

Deprecated: MongoDB\Driver\ReadPreference::__construct(): Passing an integer mode to "MongoDB\Driver\ReadPreference::__construct" is deprecated and will be removed in a future release. in %s
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
}
===DONE===
