--TEST--
MongoDB\BSON\Symbol get_properties_for handler (var_dump, print_r, array cast, var_export)
--FILE--
<?php

$symbol = MongoDB\BSON\Document::fromJSON('{ "symbol": {"$symbol": "test"} }')->toPHP()->symbol;

var_dump($symbol);

print_r($symbol);

var_dump((array) $symbol);

var_export($symbol);
echo "\n";

$restored = eval('return ' . var_export($symbol, true) . ';');
var_dump($symbol == $restored);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Symbol)#%d (1) {
  ["symbol"]=>
  string(4) "test"
}
MongoDB\BSON\Symbol Object
(
    [symbol] => test
)
array(1) {
  ["symbol"]=>
  string(4) "test"
}
%r\\?%rMongoDB\BSON\Symbol::__set_state(array(
   'symbol' => 'test',
))
bool(true)
===DONE===
