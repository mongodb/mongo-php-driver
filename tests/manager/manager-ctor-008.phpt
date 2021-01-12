--TEST--
MongoDB\Driver\Manager::__construct() does not canonicalise options
--FILE--
<?php

ini_set('mongodb.debug', 'stderr');
new MongoDB\Driver\Manager();
new MongoDB\Driver\Manager('mongodb://localhost:27017/');
ini_set('mongodb.debug', '');

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%A
[%s]     PHONGO: DEBUG   > Created client with hash: %s
%A
[%s]     PHONGO: DEBUG   > Created client with hash: %s
%A
===DONE===
