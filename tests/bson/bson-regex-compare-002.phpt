--TEST--
MongoDB\BSON\Regex comparisons (with flags)
--FILE--
<?php

var_dump(new MongoDB\BSON\Regex('regexp', 'm') == new MongoDB\BSON\Regex('regexp', 'm'));
var_dump(new MongoDB\BSON\Regex('regexp', 'm') < new MongoDB\BSON\Regex('regexp', 'm'));
var_dump(new MongoDB\BSON\Regex('regexp', 'm') > new MongoDB\BSON\Regex('regexp', 'm'));

var_dump(new MongoDB\BSON\Regex('regexp', 'm') < new MongoDB\BSON\Regex('regexp', 'x'));
var_dump(new MongoDB\BSON\Regex('regexp', 'm') > new MongoDB\BSON\Regex('regexp', 'i'));
var_dump(new MongoDB\BSON\Regex('regexp', 'm') > new MongoDB\BSON\Regex('regexp'));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(false)
bool(false)
bool(true)
bool(true)
bool(true)
===DONE===
