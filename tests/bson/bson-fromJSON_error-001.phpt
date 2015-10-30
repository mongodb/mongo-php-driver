--TEST--
BSON\fromJSON(): invalid JSON
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

echo throws(function() {
    fromJSON('foo');
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
lexical error: invalid string in json text.
                                       foo
                     (right here) ------^

===DONE===
