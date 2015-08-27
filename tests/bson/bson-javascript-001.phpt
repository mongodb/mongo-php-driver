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
    array("js" => $jswscope),
);

foreach($tests as $n => $test) {
    echo "Test#{$n}", "\n";
    $s = fromPHP($test);
    $testagain = toPHP($s);
    var_dump($test['js'] instanceof $classname);
    var_dump($testagain->js instanceof $classname);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Test#0
bool(true)
bool(true)
Test#1
bool(true)
bool(true)
===DONE===
