--TEST--
BSON encoding: Encoding data into BSON representation, and BSON into Extended JSON
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = array(
    array("hello" => "world"),
    array((object)array("hello" => "world")),
    array(null),
    array(123),
    array(4.125),
    array(true),
    array(false),
    array("string"),
    array("string", true),
    array('test', 'foo', 'bar'),
    array('test' => 'test', 'foo' => 'foo', 'bar' => 'bar'),
    array('foo' => 'test', 'foo', 'bar'),




    /*
    (object)array("hello" => "world"),
    array(array("hello" => "world")),
    array(array(1, 2, 3, 4, 5, 6, 7, 8, 9)),
    array((object)array(1, 2, 3, 4, 5, 6, 7, 8, 9)),
    array(array("0" => 1, "1" => 2, "2" => 3, "3" => 4, "4" => 5, "5" => 6, "6" => 7, "7" => 8, "8" => 9)),
    array("int" => 3, "boolean" => true, "array" => array("foo", "bar"), "object" => new stdclass, "string" => "test", 3 => "test"),
    array(array("string", true)),
    array(array('test', 'foo', 'bar')),
    array(array('test' => 'test', 'foo' => 'foo', 'bar' => 'bar')),
    array(array('foo' => 'test', 'foo', 'bar')),
    array(array("int" => 3, "boolean" => true, "array" => array("foo", "bar"), "object" => new stdclass, "string" => "test", 3 => "test")),
*/
);

foreach($tests as $n => $test) {
    $s = fromPHP($test);
    echo "Test#{$n} ", toJSON($s), "\n";
    $val = toPHP($s);
    if ($val == (object) $test) {
        echo "OK\n";
    } else {
        var_dump($val, $test);
    }
}
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Test#0 { "hello" : "world" }
OK
Test#1 { "0" : { "hello" : "world" } }
OK
Test#2 { "0" : null }
OK
Test#3 { "0" : 123 }
OK
Test#4 { "0" : 4.125 }
OK
Test#5 { "0" : true }
OK
Test#6 { "0" : false }
OK
Test#7 { "0" : "string" }
OK
Test#8 { "0" : "string", "1" : true }
OK
Test#9 { "0" : "test", "1" : "foo", "2" : "bar" }
OK
Test#10 { "test" : "test", "foo" : "foo", "bar" : "bar" }
OK
Test#11 { "foo" : "test", "0" : "foo", "1" : "bar" }
OK
===DONE===
