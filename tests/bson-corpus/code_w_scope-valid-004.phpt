--TEST--
Javascript Code with Scope: Non-empty code string and non-empty scope
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('210000000F6100190000000500000061626364000C000000107800010000000000');
$canonicalExtJson = '{"a" : {"$code" : "abcd", "$scope" : {"x" : {"$numberInt": "1"}}}}';

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

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
210000000f6100190000000500000061626364000c000000107800010000000000
210000000f6100190000000500000061626364000c000000107800010000000000
{"a":{"$code":"abcd","$scope":{"x":{"$numberInt":"1"}}}}
{"a":{"$code":"abcd","$scope":{"x":{"$numberInt":"1"}}}}
210000000f6100190000000500000061626364000c000000107800010000000000
210000000f6100190000000500000061626364000c000000107800010000000000
===DONE===