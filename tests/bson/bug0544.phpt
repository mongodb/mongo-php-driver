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
    $bson = MongoDB\BSON\fromPHP($test);
    /* Note: Although libbson can parse the extended JSON representation for
     * 64-bit integers (i.e. "$numberLong"), it currently prints them as
     * doubles (see: https://jira.mongodb.org/browse/CDRIVER-375). */
    printf("Test %s\n", MongoDB\BSON\toJSON($bson));
    hex_dump($bson);
    var_dump(MongoDB\BSON\toPHP($bson));
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toJSON() is deprecated in %s
Test { "x" : -2147483648 }
     0 : 0c 00 00 00 10 78 00 00 00 00 80 00              [.....x......]

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
object(stdClass)#%d (%d) {
  ["x"]=>
  int(-2147483648)
}


Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toJSON() is deprecated in %s
Test { "x" : 2147483647 }
     0 : 0c 00 00 00 10 78 00 ff ff ff 7f 00              [.....x......]

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
object(stdClass)#%d (%d) {
  ["x"]=>
  int(2147483647)
}


Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toJSON() is deprecated in %s
Test { "x" : -4294967294 }
     0 : 10 00 00 00 12 78 00 02 00 00 00 ff ff ff ff 00  [.....x..........]

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
object(stdClass)#%d (%d) {
  ["x"]=>
  int(-4294967294)
}


Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toJSON() is deprecated in %s
Test { "x" : 4294967294 }
     0 : 10 00 00 00 12 78 00 fe ff ff ff 00 00 00 00 00  [.....x..........]

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
object(stdClass)#%d (%d) {
  ["x"]=>
  int(4294967294)
}


Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toJSON() is deprecated in %s
Test { "x" : -4294967295 }
     0 : 10 00 00 00 12 78 00 01 00 00 00 ff ff ff ff 00  [.....x..........]

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
object(stdClass)#%d (%d) {
  ["x"]=>
  int(-4294967295)
}


Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toJSON() is deprecated in %s
Test { "x" : 4294967295 }
     0 : 10 00 00 00 12 78 00 ff ff ff ff 00 00 00 00 00  [.....x..........]

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
object(stdClass)#%d (%d) {
  ["x"]=>
  int(4294967295)
}


Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toJSON() is deprecated in %s
Test { "x" : -9223372036854775807 }
     0 : 10 00 00 00 12 78 00 01 00 00 00 00 00 00 80 00  [.....x..........]

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
object(stdClass)#%d (%d) {
  ["x"]=>
  int(-9223372036854775807)
}


Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toJSON() is deprecated in %s
Test { "x" : 9223372036854775807 }
     0 : 10 00 00 00 12 78 00 ff ff ff ff ff ff ff 7f 00  [.....x..........]

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
object(stdClass)#%d (%d) {
  ["x"]=>
  int(9223372036854775807)
}

===DONE===
