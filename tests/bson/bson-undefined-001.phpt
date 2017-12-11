--TEST--
MongoDB\BSON\Undefined #001
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = array(
    MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "undefined": {"$undefined": true} }')),
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
Test#0 { "undefined" : { "$undefined" : true } }
string(41) "{ "undefined" : { "$undefined" : true } }"
string(41) "{ "undefined" : { "$undefined" : true } }"
bool(true)
===DONE===
