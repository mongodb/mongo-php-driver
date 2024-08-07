--TEST--
Decimal128: [decq176] Nmin and below
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('18000000136400010000000A5BC138938D44C64D31008000');
$canonicalExtJson = '{"d" : {"$numberDecimal" : "-1.000000000000000000000000000000001E-6143"}}';

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
18000000136400010000000a5bc138938d44c64d31008000
{"d":{"$numberDecimal":"-1.000000000000000000000000000000001E-6143"}}
18000000136400010000000a5bc138938d44c64d31008000
===DONE===