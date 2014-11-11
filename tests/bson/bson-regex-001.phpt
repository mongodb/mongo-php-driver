--TEST--
BSON BSON\Regex #001
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php 
require_once "tests/utils/basic.inc";

$regexp = new BSON\Regex("regexp", "i");
printf("Pattern: %s\n", $regexp->getPattern());
printf("Flags: %s\n", $regexp->getFlags());
printf("String representation: %s\n", $regexp);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Pattern: regexp
Flags: i
String representation: /regexp/i
===DONE===
