--TEST--
PHPC-357: The exception for "invalid namespace" does not list the broken name
--FILE--
<?php
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
