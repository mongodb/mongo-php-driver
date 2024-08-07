--TEST--
Regular Expression type: Required escapes
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('100000000B610061625C226162000000');
$canonicalExtJson = '{"a" : {"$regularExpression" : { "pattern": "ab\\\\\\"ab", "options" : ""}}}';

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
100000000b610061625c226162000000
{"a":{"$regularExpression":{"pattern":"ab\\\"ab","options":""}}}
100000000b610061625c226162000000
===DONE===