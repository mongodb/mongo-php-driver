--TEST--
MongoDB\BSON\Int64 wraps 64-bit integers on 32-bit platforms
--SKIPIF--
<?php if (4 !== PHP_INT_SIZE) { die('skip Only for 32-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    '{"max64": {"$numberLong": "9223372036854775807"}}',
    '{"min64": {"$numberLong": "-9223372036854775808"}}',
    '{"max32+1": {"$numberLong": "2147483648"}}',
    '{"min32-1": {"$numberLong": "-2147483649"}}',
    // Numbers within the range of 32-bit integers will not be wrapped
    '{"max32": {"$numberLong": "2147483647"}}',
    '{"min32": {"$numberLong": "-2147483648"}}',
    '{"zero": {"$numberLong": "0"}}',
    
];

foreach ($tests as $test) {
    var_dump(MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON($test)));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
object(stdClass)#%d (%d) {
  ["max64"]=>
  object(MongoDB\BSON\Int64)#%d (%d) {
    ["integer"]=>
    string(19) "9223372036854775807"
  }
}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
object(stdClass)#%d (%d) {
  ["min64"]=>
  object(MongoDB\BSON\Int64)#%d (%d) {
    ["integer"]=>
    string(20) "-9223372036854775808"
  }
}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
object(stdClass)#%d (%d) {
  ["max32+1"]=>
  object(MongoDB\BSON\Int64)#%d (%d) {
    ["integer"]=>
    string(10) "2147483648"
  }
}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
object(stdClass)#%d (%d) {
  ["min32-1"]=>
  object(MongoDB\BSON\Int64)#%d (%d) {
    ["integer"]=>
    string(11) "-2147483649"
  }
}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
object(stdClass)#%d (%d) {
  ["max32"]=>
  int(2147483647)
}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
object(stdClass)#%d (%d) {
  ["min32"]=>
  int(-2147483648)
}

Deprecated: Function MongoDB\BSON\fromJSON() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
object(stdClass)#%d (%d) {
  ["zero"]=>
  int(0)
}
===DONE===
