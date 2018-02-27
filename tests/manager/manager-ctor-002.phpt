--TEST--
MongoDB\Driver\Manager::__construct() with URI
--FILE--
<?php

$manager = new MongoDB\Driver\Manager('mongodb://127.0.0.1/');

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===DONE===
