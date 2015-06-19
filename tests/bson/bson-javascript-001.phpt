--TEST--
BSON MongoDB\BSON\Javascript #001
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$js = new MongoDB\BSON\Javascript("function foo(bar) {var baz = bar; var bar = foo; return bar; }");
$jswscope = new MongoDB\BSON\Javascript("function foo(bar) {var baz = bar; var bar = foo; return bar; }", array("foo" => 42));
$tests = array(
    array("js" => $js),
    array("jswscope" => $jswscope),
);

throws(function() {
    $j = new MongoDB\BSON\Javascript;
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");

foreach($tests as $n => $test) {
    echo "Test#{$n}", "\n";
    $s = MongoDB\BSON\fromArray($test);
    $testagain = MongoDB\BSON\toArray($s);
    var_dump(current($test) instanceof MongoDB\BSON\Javascript);
    var_dump(current($testagain) instanceof MongoDB\BSON\Javascript);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Test#0
bool(true)
bool(true)
Test#1
bool(true)
bool(true)
===DONE===
