--TEST--
MongoDB\Driver\Manager::__construct() URI defaults to "mongodb://127.0.0.1/"
--FILE--
<?php

ini_set('mongodb.debug', 'stderr');
$manager = new MongoDB\Driver\Manager();
ini_set('mongodb.debug', '');

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
[%s]     PHONGO: DEBUG   > Connection string: 'mongodb://127.0.0.1/'
%A
===DONE===
