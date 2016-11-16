--TEST--
MongoDB\BSON\Javascript::jsonSerialize() return value (without scope)
--FILE--
<?php

$js = new MongoDB\BSON\Javascript('function foo(bar) { return bar; }');
var_dump($js->jsonSerialize());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(1) {
  ["$code"]=>
  string(33) "function foo(bar) { return bar; }"
}
===DONE===
