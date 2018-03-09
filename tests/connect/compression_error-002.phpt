--TEST--
MongoDB\Driver\Manager: Connecting with invalid compressor values
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('STANDALONE'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(STANDALONE, [ 'compressors' => "foo\xFEbar"] );
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected invalid UTF-8 for field path "compressors": %s
===DONE===
