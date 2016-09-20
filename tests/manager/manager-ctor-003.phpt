--TEST--
MongoDB\Driver\Manager::__construct() URI defaults to "mongodb://127.0.0.1/"
--SKIPIF--
<?php if (defined("HHVM_VERSION_ID")) exit("skip HHVM uses HHVM's logging functionality, and hence can't catch this"); ?>
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
