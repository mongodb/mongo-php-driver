--TEST--
Symbol: Embedded nulls
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('190000000E61000D0000006162006261620062616261620000');
$convertedBson = hex2bin('190000000261000D0000006162006261620062616261620000');
$canonicalExtJson = '{"a": {"$symbol": "ab\\u0000bab\\u0000babab"}}';
$convertedExtJson = '{"a": "ab\\u0000bab\\u0000babab"}';

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
190000000e61000d0000006162006261620062616261620000
190000000e61000d0000006162006261620062616261620000
{"a":{"$symbol":"ab\u0000bab\u0000babab"}}
{"a":{"$symbol":"ab\u0000bab\u0000babab"}}
190000000e61000d0000006162006261620062616261620000
190000000e61000d0000006162006261620062616261620000
===DONE===