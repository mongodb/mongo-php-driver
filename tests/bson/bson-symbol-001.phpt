--TEST--
MongoDB\BSON\Symbol #001
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = array(
    MongoDB\BSON\Document::fromJSON('{ "symbol": {"$symbol": "test"} }')->toPHP(),
);

foreach($tests as $n => $test) {
    $s = fromPHP($test);
    echo "Test#{$n} ", $json = toJSON($s), "\n";
    $testagain = toPHP($s);
    var_dump(toJSON(fromPHP($test)), toJSON(fromPHP($testagain)));
    var_dump((object)$test == (object)$testagain);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Test#0 { "symbol" : { "$symbol" : "test" } }
string(37) "{ "symbol" : { "$symbol" : "test" } }"
string(37) "{ "symbol" : { "$symbol" : "test" } }"
bool(true)
===DONE===
