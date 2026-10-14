--TEST--
PHPC-276: Recursive BSON structures do not exhaust the stack
--DESCRIPTION--
Encoding the result of a recursive bsonSerialize() method used to recurse in C
until the stack was exhausted and the process crashed. The same happened when
decoding a deeply nested BSON document. Both paths now throw an
UnexpectedValueException before the stack is exhausted.

The encoding limit is 100 levels (BSON_MAX_NESTING_LEVEL, matching libbson's
JSON parser). The decoding limit is higher (BSON_MAX_DECODE_NESTING_LEVEL,
matching libbson's BSON_VALIDATION_MAX_NESTING_DEPTH).
The server has a limit of 100 levels, but users may decode BSON directly. 
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

class MyRecursiveSerializable implements MongoDB\BSON\Serializable
{
    private int $depth;

    public function __construct(int $depth)
    {
        $this->depth = $depth;
    }

    public function bsonSerialize(): array
    {
        return $this->depth >= 1 ? [new self($this->depth - 1)] : ['end'];
    }
}

/* Builds a BSON document nesting a document under "a", $depth levels deep. */
function nestedDocument(int $depth): string
{
    $doc = bsonDocument([bsonString('end', 'x')]);

    for ($i = 0; $i < $depth; $i++) {
        $doc = bsonDocument(["\x03" . "a\x00" . $doc]);
    }

    return $doc;
}

/* Same as nestedDocument(), but nests BSON arrays instead of documents. */
function nestedArray(int $depth): string
{
    $arr = bsonDocument([bsonString('0', 'end')]);

    for ($i = 0; $i < $depth; $i++) {
        $arr = bsonDocument(["\x04" . "0\x00" . $arr]);
    }

    return $arr;
}

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

function bsonString(string $key, string $value): string
{
    return "\x02" . $key . "\x00" . pack('V', strlen($value) + 1) . $value . "\x00";
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

echo "Encoding a recursive bsonSerialize() result\n";
echo throws(function() {
    MongoDB\BSON\Document::fromPHP(new MyRecursiveSerializable(50000));
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

echo "Decoding a document nested 500 levels deep\n";
MongoDB\BSON\Document::fromBSON(nestedDocument(500))->toPHP();

echo "Decoding a document nested 501 levels deep\n";
echo throws(function() {
    MongoDB\BSON\Document::fromBSON(nestedDocument(501))->toPHP();
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

echo "Decoding an array nested 500 levels deep\n";
MongoDB\BSON\Document::fromBSON(nestedArray(500))->toPHP();

echo "Decoding an array nested 501 levels deep\n";
echo throws(function() {
    MongoDB\BSON\Document::fromBSON(nestedArray(501))->toPHP();
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

/* A CodeWScope scope is decoded with its own field path, so the nesting depth
 * must be carried over explicitly. */
echo "Decoding a CodeWScope nested 500 levels deep\n";
MongoDB\BSON\Document::fromBSON(nestedCodeWithScope(500))->toPHP();

echo "Decoding a CodeWScope nested 501 levels deep\n";
echo throws(function() {
    MongoDB\BSON\Document::fromBSON(nestedCodeWithScope(501))->toPHP();
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

/* A raw BSON sub-document is not traversed, so it does not count against the
 * nesting limit even at the deepest level. */
echo "Decoding a 501-level document with a raw BSON leaf\n";
MongoDB\BSON\Document::fromBSON(nestedDocument(501))->toPHP([
    'fieldPaths' => [implode('.', array_fill(0, 501, 'a')) => 'bson'],
]);

echo "Decoding a 501-level array with a raw BSON leaf\n";
MongoDB\BSON\Document::fromBSON(nestedArray(501))->toPHP([
    'fieldPaths' => [implode('.', array_fill(0, 501, '0')) => 'bson'],
]);

/* Well past the point at which the C recursion would exhaust the stack. */
echo "Decoding a document nested 20000 levels deep\n";
echo throws(function() {
    MongoDB\BSON\Document::fromBSON(nestedDocument(20000))->toPHP();
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Encoding a recursive bsonSerialize() result
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Nesting level too deep
Decoding a document nested 500 levels deep
Decoding a document nested 501 levels deep
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Nesting level too deep for field path "a%sa"
Decoding an array nested 500 levels deep
Decoding an array nested 501 levels deep
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Nesting level too deep for field path "0%s0"
Decoding a CodeWScope nested 500 levels deep
Decoding a CodeWScope nested 501 levels deep
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Nesting level too deep for field path "a"
Decoding a 501-level document with a raw BSON leaf
Decoding a 501-level array with a raw BSON leaf
Decoding a document nested 20000 levels deep
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Nesting level too deep for field path "a%sa"
===DONE===
