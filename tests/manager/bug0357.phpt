--TEST--
PHPC-357: The exception for "invalid namespace" does not list the broken name
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";
$m = new MongoDB\Driver\Manager("mongodb://localhost:44444");

$c = new MongoDB\Driver\Query( [] );

try {
	$m->executeQuery( 'demo', $c );
	echo "Expected exception not thrown\n";
}
catch ( InvalidArgumentException $e )
{
	echo $e->getMessage(), "\n";
}
?>
--EXPECTF--
Invalid namespace provided: demo
