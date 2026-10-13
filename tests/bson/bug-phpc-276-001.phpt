--TEST--
PHPC-276: Fetching a deeply nested CodeWScope value does not exhaust the stack
--DESCRIPTION--
A standalone BSON value is decoded one level below its document. When the value
is a CodeWScope, its scope is decoded eagerly, so the decode nesting limit must
count from that level. Fetching the value directly used to start the scope at
depth 0 and accept one level more than a full document decode.
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* Builds a document nesting a CodeWScope under "a", $depth levels deep. Each
 * scope is a document decoded with its own field path. */
function nestedCodeWithScope(int $depth): string
{
    $doc = bsonDocument([]);

    for ($i = 0; $i < $depth; $i++) {
        $doc = bsonDocument([bsonCodeWithScope('a', 'x', $doc)]);
    }

    return $doc;
}

function bsonCodeWithScope(string $key, string $code, string $scopeDocument): string
{
    $codeString = pack('V', strlen($code) + 1) . $code . "\x00";
    $length     = 4 + strlen($codeString) + strlen($scopeDocument);

    return "\x0F" . $key . "\x00" . pack('V', $length) . $codeString . $scopeDocument;
}

function bsonDocument(array $elements): string
{
    $body = implode('', $elements);

    return pack('V', 4 + strlen($body) + 1) . $body . "\x00";
}

echo "Fetching a CodeWScope nested 500 levels deep directly\n";
MongoDB\BSON\Document::fromBSON(nestedCodeWithScope(500))->get('a');

echo "Fetching a CodeWScope nested 501 levels deep directly\n";
echo throws(function() {
    MongoDB\BSON\Document::fromBSON(nestedCodeWithScope(501))->get('a');
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fetching a CodeWScope nested 500 levels deep directly
Fetching a CodeWScope nested 501 levels deep directly
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Nesting level too deep for field path "a"
===DONE===
