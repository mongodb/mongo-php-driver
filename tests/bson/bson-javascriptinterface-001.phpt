--TEST--
MongoDB\BSON\JavascriptInterface is implemented by MongoDB\BSON\Javascript
--FILE--
<?php

$javascript = new MongoDB\BSON\Javascript('function foo() { return bar; }', ['bar' => 1]);
var_dump($javascript instanceof MongoDB\BSON\JavascriptInterface);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===
