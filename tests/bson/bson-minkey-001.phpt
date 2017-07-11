--TEST--
MongoDB\BSON\MinKey #001
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$minkey = new MongoDB\BSON\MinKey;
$tests = array(
    array("min" => $minkey),
);

foreach($tests as $n => $test) {
    $s = fromPHP($test);
    echo "Test#{$n} ", $json = toJSON($s), "\n";
    $bson = fromJSON($json);
    $testagain = toPHP($bson);
    var_dump(toJSON(fromPHP($test)), toJSON(fromPHP($testagain)));
    var_dump((object)$test == (object)$testagain);
}
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Test#0 { "min" : { "$minKey" : 1 } }
string(29) "{ "min" : { "$minKey" : 1 } }"
string(29) "{ "min" : { "$minKey" : 1 } }"
bool(true)
===DONE===
