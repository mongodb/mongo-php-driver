--TEST--
MongoDB\Driver\Manager::__construct() reuses cached mongoc client
--FILE--
<?php

ini_set('mongodb.debug', 'stderr');
new MongoDB\Driver\Manager();
new MongoDB\Driver\Manager();
ini_set('mongodb.debug', '');

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%A
[%s]     PHONGO: DEBUG   > Created client with hash: %s
%A
[%s]     PHONGO: DEBUG   > Found client for hash: %s
%A
===DONE===
