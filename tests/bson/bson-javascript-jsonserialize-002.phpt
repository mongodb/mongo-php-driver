--TEST--
MongoDB\BSON\Javascript::jsonSerialize() return value (with scope)
--FILE--
<?php

$js = new MongoDB\BSON\Javascript('function(bar) { return bar; }', ['foo' => 42]);
var_dump($js->jsonSerialize());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(2) {
  ["$code"]=>
  string(29) "function(bar) { return bar; }"
  ["$scope"]=>
  object(stdClass)#%d (%d) {
    ["foo"]=>
    int(42)
  }
}
===DONE===
