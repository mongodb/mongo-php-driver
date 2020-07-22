--TEST--
Decimal128: [decq416] clamped zeros... (Clamped)
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$canonicalBson = hex2bin('180000001364000000000000000000000000000000FE5F00');
$canonicalExtJson = '{"d" : {"$numberDecimal" : "0E+6111"}}';
$degenerateExtJson = '{"d" : {"$numberDecimal" : "0E+6144"}}';

// Canonical BSON -> Native -> Canonical BSON
echo bin2hex(fromPHP(toPHP($canonicalBson))), "\n";

// Canonical BSON -> Canonical extJSON
echo json_canonicalize(toCanonicalExtendedJSON($canonicalBson)), "\n";

// Canonical extJSON -> Canonical BSON
echo bin2hex(fromJSON($canonicalExtJson)), "\n";

// Degenerate extJSON -> Canonical BSON
echo bin2hex(fromJSON($degenerateExtJson)), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
180000001364000000000000000000000000000000fe5f00
{"d":{"$numberDecimal":"0E+6111"}}
180000001364000000000000000000000000000000fe5f00
180000001364000000000000000000000000000000fe5f00
===DONE===