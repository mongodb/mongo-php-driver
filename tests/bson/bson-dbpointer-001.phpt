--TEST--
MongoDB\BSON\DBPointer #001
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = array(
    MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "dbref": {"$dbPointer": {"$ref": "phongo.test", "$id" : { "$oid" : "5a2e78accd485d55b405ac12" }  }} }')),
);

foreach($tests as $n => $test) {
    echo "Test#{$n}", "\n";
    $s = fromPHP($test);
    $testagain = toPHP($s);
    var_dump($test->dbref instanceof MongoDB\BSON\DBPointer);
    var_dump($testagain->dbref instanceof MongoDB\BSON\DBPointer);
    var_dump(toJSON(fromPHP($test)), toJSON(fromPHP($testagain)));
    var_dump((object)$test == (object)$testagain);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Test#0
bool(true)
bool(true)
string(76) "{ "dbref" : { "$ref" : "phongo.test", "$id" : "5a2e78accd485d55b405ac12" } }"
string(76) "{ "dbref" : { "$ref" : "phongo.test", "$id" : "5a2e78accd485d55b405ac12" } }"
bool(true)
===DONE===
