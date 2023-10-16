--TEST--
MongoDB\BSON\Javascript::jsonSerialize() return value (without scope)
--FILE--
<?php

$js = new MongoDB\BSON\Javascript('function(bar) { return bar; }');
var_dump($js->jsonSerialize());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(1) {
  ["$code"]=>
  string(29) "function(bar) { return bar; }"
}
===DONE===
