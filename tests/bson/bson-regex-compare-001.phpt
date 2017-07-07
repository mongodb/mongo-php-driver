--TEST--
MongoDB\BSON\Regex comparisons (without flags)
--FILE--
<?php

var_dump(new MongoDB\BSON\Regex('regexp') == new MongoDB\BSON\Regex('regexp'));
var_dump(new MongoDB\BSON\Regex('regexp') < new MongoDB\BSON\Regex('regexp'));
var_dump(new MongoDB\BSON\Regex('regexp') > new MongoDB\BSON\Regex('regexp'));

var_dump(new MongoDB\BSON\Regex('regexp') < new MongoDB\BSON\Regex('regexr'));
var_dump(new MongoDB\BSON\Regex('regexp') > new MongoDB\BSON\Regex('regexo'));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(false)
bool(false)
bool(true)
bool(true)
===DONE===
