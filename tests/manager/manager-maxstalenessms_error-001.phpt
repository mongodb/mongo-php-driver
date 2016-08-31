--TEST--
MongoDB\Driver\Manager: maxStalenessMS
--FILE--
<?php
try {
	$manager = new MongoDB\Driver\Manager("mongodb://localhost/?maxstalenessms=1231");
} catch ( MongoDB\Driver\Exception\InvalidArgumentException $e ) {
	echo $e->getMessage(), "\n";
}

try {
	$manager = new MongoDB\Driver\Manager("mongodb://localhost/?maxStalenessMS=1231");
} catch ( MongoDB\Driver\Exception\InvalidArgumentException $e ) {
	echo $e->getMessage(), "\n";
}

try {
	$manager = new MongoDB\Driver\Manager("mongodb://localhost/", [ 'maxstalenessms' => 1231 ] );
} catch ( MongoDB\Driver\Exception\InvalidArgumentException $e ) {
	echo $e->getMessage(), "\n";
}

try {
	$manager = new MongoDB\Driver\Manager("mongodb://localhost/", [ 'maxStalenessMS' => 1231 ] );
} catch ( MongoDB\Driver\Exception\InvalidArgumentException $e ) {
	echo $e->getMessage(), "\n";
}
?>
--EXPECTF--
Failed to parse MongoDB URI: 'mongodb://localhost/?maxstalenessms=1231'
Failed to parse MongoDB URI: 'mongodb://localhost/?maxStalenessMS=1231'
Primary read preference mode conflicts with maxStalenessMS
Primary read preference mode conflicts with maxStalenessMS
