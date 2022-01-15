--TEST--
MongoDB\BSON\toJSON(): Encoding JSON with duplicate field names
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* Note: this is a regression test for libmongoc's JSON encoder. It is not
 * advisable to use duplicate keys in BSON documents. */
echo toJSON(fromJSON('{ "foo": "bar", "foo": "baz" }')), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
{ "foo" : "bar", "foo" : "baz" }
===DONE===
