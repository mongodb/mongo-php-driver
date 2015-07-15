--TEST--
BSON BSON\Javascript #001
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$classname = BSON_NAMESPACE . "\\Javascript";

$js = new $classname("function foo(bar) {var baz = bar; var bar = foo; return bar; }");
$jswscope = new $classname("function foo(bar) {var baz = bar; var bar = foo; return bar; }", array("foo" => 42));
$tests = array(
    array("js" => $js),
    array("jswscope" => $jswscope),
);

throws(function() use($classname) {
    $j = new $classname;
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");

foreach($tests as $n => $test) {
    echo "Test#{$n}", "\n";
    $s = fromPHP($test);
    $testagain = toPHP($s);
    var_dump(current($test) instanceof $classname);
    var_dump(current($testagain) instanceof $classname);
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
