--TEST--
BSON BSON\Timestamp #001 error
--SKIPIF--
<?php if (defined("HHVM_VERSION_ID")) exit("skip HHVM handles parameter parsing differently"); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

throws(function() {
    $classname = BSON_NAMESPACE . "\\Timestamp";
    new $classname;
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
===DONE===

