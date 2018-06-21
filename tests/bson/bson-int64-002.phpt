--TEST--
MongoDB\BSON\Int64 wraps 64-bit integers on 32-bit platforms
--SKIPIF--
<?php if (4 !== PHP_INT_SIZE) { die('skip Only for 32-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{"max64": {"$numberLong": "9223372036854775807"}}')),
    MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{"min64": {"$numberLong": "-9223372036854775808"}}')),
    MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{"max32+1": {"$numberLong": "2147483648"}}')),
    MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{"min32-1": {"$numberLong": "-2147483649"}}')),
    // Numbers within the range of 32-bit integers will not be wrapped
    MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{"max32": {"$numberLong": "2147483647"}}')),
    MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{"min32": {"$numberLong": "-2147483648"}}')),
    MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{"zero": {"$numberLong": "0"}}')),
    
];

foreach ($tests as $test) {
    var_dump($test);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["max64"]=>
  object(MongoDB\BSON\Int64)#%d (%d) {
    ["integer"]=>
    string(19) "9223372036854775807"
  }
}
object(stdClass)#%d (%d) {
  ["min64"]=>
  object(MongoDB\BSON\Int64)#%d (%d) {
    ["integer"]=>
    string(20) "-9223372036854775808"
  }
}
object(stdClass)#%d (%d) {
  ["max32+1"]=>
  object(MongoDB\BSON\Int64)#%d (%d) {
    ["integer"]=>
    string(10) "2147483648"
  }
}
object(stdClass)#%d (%d) {
  ["min32-1"]=>
  object(MongoDB\BSON\Int64)#%d (%d) {
    ["integer"]=>
    string(11) "-2147483649"
  }
}
object(stdClass)#%d (%d) {
  ["max32"]=>
  int(2147483647)
}
object(stdClass)#%d (%d) {
  ["min32"]=>
  int(-2147483648)
}
object(stdClass)#%d (%d) {
  ["zero"]=>
  int(0)
}
===DONE===
