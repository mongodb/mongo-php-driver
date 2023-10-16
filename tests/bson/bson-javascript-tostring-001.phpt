--TEST--
MongoDB\BSON\Javascript::__toString()
--FILE--
<?php

$js = new MongoDB\BSON\Javascript('function() { return 1; }');
var_dump((string) $js);

$js = new MongoDB\BSON\Javascript('function() { return bar; }', ['bar' => 1]);
var_dump((string) $js);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(24) "function() { return 1; }"
string(26) "function() { return bar; }"
===DONE===
