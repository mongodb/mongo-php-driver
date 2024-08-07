--TEST--
Top-level document validity: Bad $timestamp ('t' type is string, not number)
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

throws(function() {
    MongoDB\BSON\Document::fromJSON('{"a" : {"$timestamp" : {"t" : "123456789", "i" : 42} } }');
}, 'MongoDB\Driver\Exception\UnexpectedValueException');

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
===DONE===