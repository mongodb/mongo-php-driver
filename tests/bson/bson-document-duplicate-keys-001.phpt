--TEST--
MongoDB\BSON\Document: Encoding duplicate keys
--DESCRIPTION--
BSON corpus spec prose test #2.1 (encoding)
https://github.com/mongodb/specifications/blob/master/source/bson-corpus/bson-corpus.md#21-test-encoding
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* PHPC does not expose a BSON builder API. PHP arrays and objects cannot hold
 * duplicate keys, so the duplicate keys below are collapsed by PHP itself
 * before reaching the driver. */
$document = MongoDB\BSON\Document::fromPHP(['foo' => 1, 'foo' => 2]);

echo "PHP collapses duplicate keys, keeping the last value\n";
echo $document->toRelaxedExtendedJSON(), "\n";
var_dump((string) $document === hex2bin('0e00000010666f6f000200000000'));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
PHP collapses duplicate keys, keeping the last value
{ "foo" : 2 }
bool(true)
===DONE===
