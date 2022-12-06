--TEST--
Int64 type: MaxValue
--SKIPIF--
<?php if (PHP_INT_SIZE !== 8) { die("skip Can't represent 64-bit ints on a 32-bit platform"); } ?>
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('10000000126100FFFFFFFFFFFFFF7F00');
$canonicalExtJson = '{"a" : {"$numberLong" : "9223372036854775807"}}';
$relaxedExtJson = '{"a" : 9223372036854775807}';

// Canonical BSON -> Native -> Canonical BSON
echo bin2hex(fromPHP(toPHP($canonicalBson))), "\n";

// Canonical BSON -> BSON object -> Canonical BSON
echo bin2hex((string) MongoDB\BSON\Document::fromBSONString($canonicalBson)), "\n";

// Canonical BSON -> Canonical extJSON
echo json_canonicalize(toCanonicalExtendedJSON($canonicalBson)), "\n";

// Canonical BSON -> BSON object -> Canonical extJSON
echo json_canonicalize(MongoDB\BSON\Document::fromBSONString($canonicalBson)->toCanonicalExtendedJSON()), "\n";

// Canonical BSON -> Relaxed extJSON
echo json_canonicalize(toRelaxedExtendedJSON($canonicalBson)), "\n";

// Canonical BSON -> BSON object -> Relaxed extJSON
echo json_canonicalize(MongoDB\BSON\Document::fromBSONString($canonicalBson)->toRelaxedExtendedJSON()), "\n";

// Canonical extJSON -> Canonical BSON
echo bin2hex(fromJSON($canonicalExtJson)), "\n";

// Canonical extJSON -> BSON object -> Canonical BSON
echo bin2hex((string) MongoDB\BSON\Document::fromJSON($canonicalExtJson)), "\n";

// Relaxed extJSON -> BSON -> Relaxed extJSON
echo json_canonicalize(toRelaxedExtendedJSON(fromJSON($relaxedExtJson))), "\n";

// Relaxed extJSON -> BSON object -> Relaxed extJSON
echo json_canonicalize(MongoDB\BSON\Document::fromJSON($relaxedExtJson)->toRelaxedExtendedJSON()), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
10000000126100ffffffffffffff7f00
10000000126100ffffffffffffff7f00
{"a":{"$numberLong":"9223372036854775807"}}
{"a":{"$numberLong":"9223372036854775807"}}
{"a":9223372036854775807}
{"a":9223372036854775807}
10000000126100ffffffffffffff7f00
10000000126100ffffffffffffff7f00
{"a":9223372036854775807}
{"a":9223372036854775807}
===DONE===