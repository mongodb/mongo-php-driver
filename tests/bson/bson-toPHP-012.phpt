--TEST--
MongoDB\BSON\toPHP(): Decoding BSON with duplicate field names
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* Note: this is a regression test. It is not advisable to use duplicate keys in
 * BSON documents. */
var_dump(toPHP(fromJSON('{ "foo": "bar", "foo": "baz" }')));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["foo"]=>
  string(3) "baz"
}
===DONE===
