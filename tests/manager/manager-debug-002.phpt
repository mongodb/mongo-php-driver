--TEST--
MongoDB\Driver\Manager: mongodb.debug=stderr
--SKIPIF--
<?php if (defined("HHVM_VERSION_ID")) exit("skip HHVM uses HHVM's logging functionality"); ?>
--INI--
mongodb.debug=stderr
--FILE--
<?php

$manager = new MongoDB\Driver\Manager;

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%A[%s]     PHONGO: DEBUG   > Connection string: '%s'
[%s]     PHONGO: DEBUG   > Creating Manager, phongo-1.%d.%d%S[%s] - mongoc-1.%s(%s), libbson-1.%s(%s), php-%s
%A===DONE===%A
