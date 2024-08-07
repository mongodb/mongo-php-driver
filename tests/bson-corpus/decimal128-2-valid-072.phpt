--TEST--
Decimal128: [decq650] fold-down full sequence
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('1800000013640000CA9A3B00000000000000000000FE5F00');
$canonicalExtJson = '{"d" : {"$numberDecimal" : "1.000000000E+6120"}}';

// Canonical BSON -> BSON object -> Canonical BSON
echo bin2hex((string) MongoDB\BSON\Document::fromBSON($canonicalBson)), "\n";

// Canonical BSON -> BSON object -> Canonical extJSON
echo json_canonicalize(MongoDB\BSON\Document::fromBSON($canonicalBson)->toCanonicalExtendedJSON()), "\n";

// Canonical extJSON -> BSON object -> Canonical BSON
echo bin2hex((string) MongoDB\BSON\Document::fromJSON($canonicalExtJson)), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
1800000013640000ca9a3b00000000000000000000fe5f00
{"d":{"$numberDecimal":"1.000000000E+6120"}}
1800000013640000ca9a3b00000000000000000000fe5f00
===DONE===