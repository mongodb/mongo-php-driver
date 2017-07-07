--TEST--
MongoDB\BSON\Javascript #001
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$js = new MongoDB\BSON\Javascript("function foo(bar) {var baz = bar; var bar = foo; return bar; }");
$jswscope = new MongoDB\BSON\Javascript("function foo(bar) {var baz = bar; var bar = foo; return bar; }", array("foo" => 42));
$tests = array(
    array("js" => $js),
    array("js" => $jswscope),
);

foreach($tests as $n => $test) {
    echo "Test#{$n}", "\n";
    $s = fromPHP($test);
    $testagain = toPHP($s);
    var_dump($test['js'] instanceof MongoDB\BSON\Javascript);
    var_dump($testagain->js instanceof MongoDB\BSON\Javascript);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Test#0
bool(true)
bool(true)
Test#1
bool(true)
bool(true)
===DONE===
