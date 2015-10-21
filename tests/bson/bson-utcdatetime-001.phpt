--TEST--
BSON BSON\UTCDateTime #001
--INI--
date.timezone=America/Los_Angeles
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$classname = BSON_NAMESPACE . "\\UTCDateTime";
$utcdatetime = new $classname("1416445411987");

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(array('_id' => 1, 'x' => $utcdatetime));
$manager->executeBulkWrite(NS, $bulk);

$query = new MongoDB\Driver\Query(array('_id' => 1));
$cursor = $manager->executeQuery(NS, $query);
$results = iterator_to_array($cursor);

$tests = array(
    array($utcdatetime),
    array($results[0]->x),
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
Test#0 { "0" : { "$date" : 1416445411987 } }
string(37) "{ "0" : { "$date" : 1416445411987 } }"
string(37) "{ "0" : { "$date" : 1416445411987 } }"
bool(true)
Test#1 { "0" : { "$date" : 1416445411987 } }
string(37) "{ "0" : { "$date" : 1416445411987 } }"
string(37) "{ "0" : { "$date" : 1416445411987 } }"
bool(true)
===DONE===
