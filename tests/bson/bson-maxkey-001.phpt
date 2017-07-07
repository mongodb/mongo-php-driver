--TEST--
MongoDB\BSON\MaxKey #001
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$maxkey = new MongoDB\BSON\MaxKey;
$tests = array(
    array("max" => $maxkey),
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
Test#0 { "max" : { "$maxKey" : 1 } }
string(29) "{ "max" : { "$maxKey" : 1 } }"
string(29) "{ "max" : { "$maxKey" : 1 } }"
bool(true)
===DONE===
