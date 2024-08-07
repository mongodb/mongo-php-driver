--TEST--
MongoDB\BSON\fromPHP(): PHP documents with circular references
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo "\nTesting packed array with circular reference\n";

echo throws(function() {
    $document = ['x' => 1, 'y' => []];
    $document['y'][] = &$document['y'];
    MongoDB\BSON\fromPHP($document);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

echo "\nTesting packed array with circular reference at 3rd position\n";

echo throws(function() {
    $document = ['x' => 1, 'y' => [1, 2, 3]];
    $document['y'][] = &$document['y'];
    MongoDB\BSON\fromPHP($document);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

echo "\nTesting associative array with circular reference\n";

echo throws(function() {
    $document = ['x' => 1, 'y' => []];
    $document['y']['z'] = &$document['y'];
    MongoDB\BSON\fromPHP($document);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

echo "\nTesting associative array and nested array with circular reference\n";

echo throws(function() {
    $document = ['x' => 1, 'y' => []];
    $document['y'][0]['z'] = &$document['y'];
    MongoDB\BSON\fromPHP($document);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

echo "\nTesting object with circular reference\n";

echo throws(function() {
    $document = (object) ['x' => 1, 'y' => (object) []];
    $document->y->z = &$document->y;
    MongoDB\BSON\fromPHP($document);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

echo "\nTesting nested object with circular reference\n";

echo throws(function() {
    $document = (object) ['x' => 1, 'y' => (object) ['z' => (object) []]];
    $document->y->z->a = &$document->y;
    MongoDB\BSON\fromPHP($document);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing packed array with circular reference

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected recursion for field path "y.0"

Testing packed array with circular reference at 3rd position

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected recursion for field path "y.3"

Testing associative array with circular reference

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected recursion for field path "y.z"

Testing associative array and nested array with circular reference

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected recursion for field path "y.0.z"

Testing object with circular reference

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected recursion for field path "y.z"

Testing nested object with circular reference

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected recursion for field path "y.z.a"
===DONE===
