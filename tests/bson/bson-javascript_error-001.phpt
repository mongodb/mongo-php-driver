--TEST--
BSON BSON\Javascript #001 error
--SKIPIF--
<?php if (defined("HHVM_VERSION_ID")) exit("skip HHVM handles parameter parsing differently"); ?>
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
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
===DONE===
