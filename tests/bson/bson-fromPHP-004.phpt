--TEST--
BSON\fromPHP(): PHP documents with circular references
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo "\nTesting packed array with circular reference\n";

$document = ['x' => 1, 'y' => []];
$document['y'][] = &$document['y'];
echo toJson(fromPHP($document)), "\n";

echo "\nTesting associative array with circular reference\n";

$document = ['x' => 1, 'y' => []];
$document['y']['z'] = &$document['y'];
echo toJson(fromPHP($document)), "\n";

echo "\nTesting object with circular reference\n";

$document = (object) ['x' => 1, 'y' => (object) []];
$document->y->z = &$document->y;
echo toJson(fromPHP($document)), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing packed array with circular reference
{ "x" : 1, "y" : [ [  ] ] }

Testing associative array with circular reference
{ "x" : 1, "y" : { "z" : {  } } }

Testing object with circular reference
{ "x" : 1, "y" : { "z" : {  } } }
===DONE===
