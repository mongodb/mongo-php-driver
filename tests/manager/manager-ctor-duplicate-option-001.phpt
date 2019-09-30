--TEST--
MongoDB\Driver\Manager::__construct() with duplicate read preference option
--FILE--
<?php

$manager = new MongoDB\Driver\Manager(null, ['readPreference' => 'primary', 'readpreference' => 'secondary']);

echo $manager->getReadPreference()->getMode(), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
2
===DONE===
