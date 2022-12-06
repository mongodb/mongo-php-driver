--TEST--
Javascript Code: three-byte UTF-8 (☆)
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('190000000D61000D000000E29886E29886E29886E298860000');
$canonicalExtJson = '{"a" : {"$code" : "\\u2606\\u2606\\u2606\\u2606"}}';

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
190000000d61000d000000e29886e29886e29886e298860000
190000000d61000d000000e29886e29886e29886e298860000
{"a":{"$code":"\u2606\u2606\u2606\u2606"}}
{"a":{"$code":"\u2606\u2606\u2606\u2606"}}
190000000d61000d000000e29886e29886e29886e298860000
190000000d61000d000000e29886e29886e29886e298860000
===DONE===