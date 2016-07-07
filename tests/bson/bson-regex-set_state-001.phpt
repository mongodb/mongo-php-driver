--TEST--
MongoDB\BSON\Regex::__set_state()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

var_export(MongoDB\BSON\Regex::__set_state([
    'pattern' => 'regexp',
    'flags' => 'i',
]));
echo "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
MongoDB\BSON\Regex::__set_state(array(
   'pattern' => 'regexp',
   'flags' => 'i',
))
===DONE===
