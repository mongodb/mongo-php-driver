--TEST--
Decimal128: Regular - Smallest
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('18000000136400D204000000000000000000000000343000');
$canonicalExtJson = '{"d" : {"$numberDecimal" : "0.001234"}}';

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
18000000136400d204000000000000000000000000343000
{"d":{"$numberDecimal":"0.001234"}}
18000000136400d204000000000000000000000000343000
===DONE===