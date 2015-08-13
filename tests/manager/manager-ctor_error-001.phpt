--TEST--
MongoDB\Driver\Manager::__construct(): too many arguments
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
<?php if (defined("HHVM_VERSION_ID")) exit("skip HHVM throws Exception instead of InvalidArgumentException"); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(STANDALONE, array(), array(), 1);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\Manager::__construct() expects at most 3 parameters, 4 given
===DONE===
