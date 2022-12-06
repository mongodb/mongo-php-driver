--TEST--
Decimal128: [basx035] conform to rules and exponent will be in permitted range).
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('1800000013640015CD5B0700000000000000000000223000');
$canonicalExtJson = '{"d" : {"$numberDecimal" : "1.23456789E-7"}}';
$degenerateExtJson = '{"d" : {"$numberDecimal" : "0.000000123456789"}}';

// Canonical BSON -> Native -> Canonical BSON
echo bin2hex(fromPHP(toPHP($canonicalBson))), "\n";

// Canonical BSON -> BSON object -> Canonical BSON
echo bin2hex((string) MongoDB\BSON\Document::fromBSONString($canonicalBson)), "\n";

// Canonical BSON -> Canonical extJSON
echo json_canonicalize(toCanonicalExtendedJSON($canonicalBson)), "\n";

// Canonical BSON -> BSON object -> Canonical extJSON
echo json_canonicalize(MongoDB\BSON\Document::fromBSONString($canonicalBson)->toCanonicalExtendedJSON()), "\n";

// Canonical extJSON -> Canonical BSON
echo bin2hex(fromJSON($canonicalExtJson)), "\n";

// Canonical extJSON -> BSON object -> Canonical BSON
echo bin2hex((string) MongoDB\BSON\Document::fromJSON($canonicalExtJson)), "\n";

// Degenerate extJSON -> Canonical BSON
echo bin2hex(fromJSON($degenerateExtJson)), "\n";

// Degenerate extJSON -> BSON object -> Canonical BSON
echo bin2hex((string) MongoDB\BSON\Document::fromJSON($degenerateExtJson)), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
1800000013640015cd5b0700000000000000000000223000
1800000013640015cd5b0700000000000000000000223000
{"d":{"$numberDecimal":"1.23456789E-7"}}
{"d":{"$numberDecimal":"1.23456789E-7"}}
1800000013640015cd5b0700000000000000000000223000
1800000013640015cd5b0700000000000000000000223000
1800000013640015cd5b0700000000000000000000223000
1800000013640015cd5b0700000000000000000000223000
===DONE===