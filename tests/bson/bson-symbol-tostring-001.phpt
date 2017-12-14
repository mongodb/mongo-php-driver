--TEST--
MongoDB\BSON\Symbol::__toString()
--FILE--
<?php

$symbol = MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "symbol": {"$symbol": "symbolValue"} }'))->symbol;
var_dump((string) $symbol);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(11) "symbolValue"
===DONE===
