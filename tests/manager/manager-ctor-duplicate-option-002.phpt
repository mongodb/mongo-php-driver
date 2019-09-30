--TEST--
MongoDB\Driver\Manager::__construct() with duplicate read concern option
--FILE--
<?php

$manager = new MongoDB\Driver\Manager(null, ['readConcernLevel' => 'majority', 'readconcernlevel' => 'local']);

echo $manager->getReadConcern()->getLevel(), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
local
===DONE===
