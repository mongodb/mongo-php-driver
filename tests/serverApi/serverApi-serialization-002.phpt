--TEST--
MongoDB\Driver\ServerApi serialization (__serialize and __unserialize)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    new MongoDB\Driver\ServerApi('1'),
    new MongoDB\Driver\ServerApi('1', true),
    new MongoDB\Driver\ServerApi('1', null, true),
    new MongoDB\Driver\ServerApi('1', false, false),
];

foreach ($tests as $test) {
    var_dump($test);
    echo $s = serialize($test), "\n";
    var_dump(unserialize($s));
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\ServerApi)#%d (%d) {
  ["version"]=>
  string(1) "1"
  ["strict"]=>
  NULL
  ["deprecationErrors"]=>
  NULL
}
O:24:"MongoDB\Driver\ServerApi":3:{s:7:"version";s:1:"1";s:6:"strict";N;s:17:"deprecationErrors";N;}
object(MongoDB\Driver\ServerApi)#5 (3) {
  ["version"]=>
  string(1) "1"
  ["strict"]=>
  NULL
  ["deprecationErrors"]=>
  NULL
}

object(MongoDB\Driver\ServerApi)#%d (%d) {
  ["version"]=>
  string(1) "1"
  ["strict"]=>
  bool(true)
  ["deprecationErrors"]=>
  NULL
}
O:24:"MongoDB\Driver\ServerApi":3:{s:7:"version";s:1:"1";s:6:"strict";b:1;s:17:"deprecationErrors";N;}
object(MongoDB\Driver\ServerApi)#5 (3) {
  ["version"]=>
  string(1) "1"
  ["strict"]=>
  bool(true)
  ["deprecationErrors"]=>
  NULL
}

object(MongoDB\Driver\ServerApi)#%d (%d) {
  ["version"]=>
  string(1) "1"
  ["strict"]=>
  NULL
  ["deprecationErrors"]=>
  bool(true)
}
O:24:"MongoDB\Driver\ServerApi":3:{s:7:"version";s:1:"1";s:6:"strict";N;s:17:"deprecationErrors";b:1;}
object(MongoDB\Driver\ServerApi)#5 (3) {
  ["version"]=>
  string(1) "1"
  ["strict"]=>
  NULL
  ["deprecationErrors"]=>
  bool(true)
}

object(MongoDB\Driver\ServerApi)#%d (%d) {
  ["version"]=>
  string(1) "1"
  ["strict"]=>
  bool(false)
  ["deprecationErrors"]=>
  bool(false)
}
O:24:"MongoDB\Driver\ServerApi":3:{s:7:"version";s:1:"1";s:6:"strict";b:0;s:17:"deprecationErrors";b:0;}
object(MongoDB\Driver\ServerApi)#5 (3) {
  ["version"]=>
  string(1) "1"
  ["strict"]=>
  bool(false)
  ["deprecationErrors"]=>
  bool(false)
}

===DONE===
