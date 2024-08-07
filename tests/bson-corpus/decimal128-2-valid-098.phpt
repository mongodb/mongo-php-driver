--TEST--
Decimal128: [decq032] Nmax and similar
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('18000000136400FFFFFFFF638E8D37C087ADBE09EDFF5F00');
$canonicalExtJson = '{"d" : {"$numberDecimal" : "9.999999999999999999999999999999999E+6144"}}';

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
18000000136400ffffffff638e8d37c087adbe09edff5f00
{"d":{"$numberDecimal":"9.999999999999999999999999999999999E+6144"}}
18000000136400ffffffff638e8d37c087adbe09edff5f00
===DONE===