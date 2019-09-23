--TEST--
MongoDB\Driver\Manager::__construct() with duplicate write concern option
--FILE--
<?php

$manager = new MongoDB\Driver\Manager(null, ['wtimeoutms' => 500, 'wTimeoutMs' => 200]);

echo $manager->getWriteConcern()->getWtimeout(), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
200
===DONE===
