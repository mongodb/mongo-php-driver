--TEST--
Double type: double truncated
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$bson = hex2bin('0B0000000164000000F03F00');

throws(function() use ($bson) {
    MongoDB\BSON\Document::fromBSON($bson)->toPHP();
}, 'MongoDB\Driver\Exception\UnexpectedValueException');

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
===DONE===