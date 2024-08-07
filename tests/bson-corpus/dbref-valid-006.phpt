--TEST--
Document type (DBRef sub-documents): DBRef with additional dollar-prefixed and dotted fields
--DESCRIPTION--
Generated by scripts/convert-bson-corpus-tests.php

DO NOT EDIT THIS FILE
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$canonicalBson = hex2bin('48000000036462726566003c0000000224726566000b000000636f6c6c656374696f6e00072469640058921b3e6e32ab156a22b59e10612e62000100000010246300010000000000');
$canonicalExtJson = '{"dbref": {"$ref": "collection", "$id": {"$oid": "58921b3e6e32ab156a22b59e"}, "a.b": {"$numberInt": "1"}, "$c": {"$numberInt": "1"}}}';

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
48000000036462726566003c0000000224726566000b000000636f6c6c656374696f6e00072469640058921b3e6e32ab156a22b59e10612e62000100000010246300010000000000
{"dbref":{"$ref":"collection","$id":{"$oid":"58921b3e6e32ab156a22b59e"},"a.b":{"$numberInt":"1"},"$c":{"$numberInt":"1"}}}
48000000036462726566003c0000000224726566000b000000636f6c6c656374696f6e00072469640058921b3e6e32ab156a22b59e10612e62000100000010246300010000000000
===DONE===