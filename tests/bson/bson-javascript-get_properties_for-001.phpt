--TEST--
MongoDB\BSON\Javascript get_properties_for handler (var_dump, print_r, array cast, var_export)
--FILE--
<?php

$javascript = new MongoDB\BSON\Javascript('function(x) { return x; }', ['x' => 1]);

var_dump($javascript);

print_r($javascript);

var_dump((array) $javascript);

var_export($javascript);
echo "\n";

$restored = eval('return ' . var_export($javascript, true) . ';');
var_dump($javascript == $restored);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Javascript)#%d (2) {
  ["code"]=>
  string(25) "function(x) { return x; }"
  ["scope"]=>
  object(stdClass)#%d (1) {
    ["x"]=>
    int(1)
  }
}
MongoDB\BSON\Javascript Object
(
    [code] => function(x) { return x; }
    [scope] => stdClass Object
        (
            [x] => 1
        )

)
array(2) {
  ["code"]=>
  string(25) "function(x) { return x; }"
  ["scope"]=>
  object(stdClass)#%d (1) {
    ["x"]=>
    int(1)
  }
}
%r\\?%rMongoDB\BSON\Javascript::__set_state(array(
   'code' => 'function(x) { return x; }',
   'scope' =>%S
  (object) array(
     'x' => 1,
  ),
))
bool(true)
===DONE===
