--TEST--
MongoDB\BSON\fromJSON(): Decoding JSON with duplicate field names
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* Note: this is a regression test for libmongoc's JSON encoder. It is not
 * advisable to use duplicate keys in BSON documents. */
hex_dump(fromJSON('{ "foo": "bar", "foo": "baz" }'));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
     0 : 1f 00 00 00 02 66 6f 6f 00 04 00 00 00 62 61 72  [.....foo.....bar]
    10 : 00 02 66 6f 6f 00 04 00 00 00 62 61 7a 00 00     [..foo.....baz..]
===DONE===
