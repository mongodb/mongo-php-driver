--TEST--
MongoDB\Driver\WriteConcern construction (invalid arguments)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php if (defined("HHVM_VERSION_ID")) exit("skip HHVM throws Exception instead of InvalidArgumentException"); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

try {
    new MongoDB\Driver\WriteConcern("string", 10000, false, true, 1);
} catch(InvalidArgumentException $e) {
    echo $e->getMessage(), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
MongoDB\Driver\WriteConcern::__construct() expects at most 4 parameters, 5 given
===DONE===
