--TEST--
MongoDB\BSON\RegexInterface is implemented by MongoDB\BSON\Regex
--FILE--
<?php

$regex = new MongoDB\BSON\Regex('pattern', 'i');
var_dump($regex instanceof MongoDB\BSON\RegexInterface);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===
