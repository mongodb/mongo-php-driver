--TEST--
MongoDB\BSON\Symbol::__set_state()
--FILE--
<?php

$symbol = MongoDB\BSON\Document::fromJSON('{ "symbol": {"$symbol": "test"} }')->toPHP()->symbol;

$s = var_export($symbol, true);
echo $s, "\n";

var_dump(eval('return ' . $s . ';'));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%r\\?%rMongoDB\BSON\Symbol::__set_state(array(
   'symbol' => 'test',
))
object(MongoDB\BSON\Symbol)#%d (%d) {
  ["symbol"]=>
  string(4) "test"
}
===DONE===
