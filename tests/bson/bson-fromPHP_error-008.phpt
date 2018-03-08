--TEST--
MongoDB\BSON\fromPHP(): PHP documents with circular references
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo "\nTesting packed array with circular reference\n";

echo throws(function() {
    $document = ['x' => 1, 'y' => [1, 2, 3]];
    $document['y'][] = &$document['y'];
    fromPHP($document);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

echo "\nTesting associative array with circular reference\n";

echo throws(function() {
    $document = ['x' => 1, 'y' => []];
    $document['y'][0]['z'] = &$document['y'];
    fromPHP($document);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

echo "\nTesting object with circular reference\n";

echo throws(function() {
    $document = (object) ['x' => 1, 'y' => (object) []];
    $document->y->z = &$document->y;
    fromPHP($document);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Testing packed array with circular reference
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected recursion for field path "y.3"

Testing associative array with circular reference
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected recursion for field path "y.0.z"

Testing object with circular reference
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected recursion for field path "y.z"
===DONE===
