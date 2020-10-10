--TEST--
MongoDB\BSON\Regex #001 error
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '7.99'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$regexp = new MongoDB\BSON\Regex("regexp", "i");
$regexp->getPattern(true);
$regexp->getFlags(true);

throws(function() {
    new MongoDB\BSON\Regex;
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Warning: %s\Regex::getPattern() expects exactly 0 parameters, 1 given in %s on line %d

Warning: %s\Regex::getFlags() expects exactly 0 parameters, 1 given in %s on line %d
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
===DONE===
