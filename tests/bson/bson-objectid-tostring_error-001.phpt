--TEST--
MongoDB\BSON\ObjectId raises warning on invalid arguments
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '7.99'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$pregenerated = new MongoDB\BSON\ObjectId("53e28b650640fd3162152de1");

raises(function() use($pregenerated) {
    $pregenerated->__toString(1);
}, E_WARNING);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got E_WARNING
===DONE===
