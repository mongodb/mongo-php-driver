--TEST--
Javascript Code with Scope: field length too short (less than minimum size)
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$bson = hex2bin('160000000F61000D0000000100000000050000000000');

throws(function() use ($bson) {
    MongoDB\BSON\Document::fromBSON($bson)->toPHP();
}, 'MongoDB\Driver\Exception\UnexpectedValueException');

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
===DONE===