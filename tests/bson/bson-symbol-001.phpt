--TEST--
MongoDB\BSON\Symbol #001
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = array(
    MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "symbol": {"$symbol": "test"} }')),
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
Test#0 { "symbol" : "test" }
string(21) "{ "symbol" : "test" }"
string(21) "{ "symbol" : "test" }"
bool(true)
===DONE===
