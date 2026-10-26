--TEST--
PHPC-544: Consult SIZEOF_ZEND_LONG for 64-bit integer support
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    ['x' => -2147483648],
    ['x' => 2147483647],
    ['x' => -4294967294],
    ['x' => 4294967294],
    ['x' => -4294967295],
    ['x' => 4294967295],
    ['x' => -9223372036854775807],
    ['x' => 9223372036854775807],
];

foreach ($tests as $test) {
    $bson = MongoDB\BSON\Document::fromPHP($test);
    /* Note: Although libbson can parse the extended JSON representation for
     * 64-bit integers (i.e. "$numberLong"), it currently prints them as
     * doubles (see: https://jira.mongodb.org/browse/CDRIVER-375). */
    printf("Test %s\n", json_encode($test));
    var_dump($bson);
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Test {"x":-2147483648}
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(16) "DAAAABB4AAAAAIAA"
  ["value"]=>
  object(stdClass)#%d (%d) {
    ["x"]=>
    int(-2147483648)
  }
}

Test {"x":2147483647}
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(16) "DAAAABB4AP///38A"
  ["value"]=>
  object(stdClass)#%d (%d) {
    ["x"]=>
    int(2147483647)
  }
}

Test {"x":-4294967294}
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(24) "EAAAABJ4AAIAAAD/////AA=="
  ["value"]=>
  object(stdClass)#%d (%d) {
    ["x"]=>
    int(-4294967294)
  }
}

Test {"x":4294967294}
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(24) "EAAAABJ4AP7///8AAAAAAA=="
  ["value"]=>
  object(stdClass)#%d (%d) {
    ["x"]=>
    int(4294967294)
  }
}

Test {"x":-4294967295}
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(24) "EAAAABJ4AAEAAAD/////AA=="
  ["value"]=>
  object(stdClass)#%d (%d) {
    ["x"]=>
    int(-4294967295)
  }
}

Test {"x":4294967295}
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(24) "EAAAABJ4AP////8AAAAAAA=="
  ["value"]=>
  object(stdClass)#%d (%d) {
    ["x"]=>
    int(4294967295)
  }
}

Test {"x":-9223372036854775807}
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(24) "EAAAABJ4AAEAAAAAAACAAA=="
  ["value"]=>
  object(stdClass)#%d (%d) {
    ["x"]=>
    int(-9223372036854775807)
  }
}

Test {"x":9223372036854775807}
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(24) "EAAAABJ4AP////////9/AA=="
  ["value"]=>
  object(stdClass)#%d (%d) {
    ["x"]=>
    int(9223372036854775807)
  }
}

===DONE===
