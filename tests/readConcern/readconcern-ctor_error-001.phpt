--TEST--
MongoDB\Driver\ReadConcern construction (invalid arguments)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php if (defined("HHVM_VERSION_ID")) exit("skip HHVM throws Exception instead of InvalidArgumentException"); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

echo throws(function() {
    new MongoDB\Driver\ReadConcern("string", 1);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ReadConcern::__construct() expects at most 1 parameter, 2 given
===DONE===
