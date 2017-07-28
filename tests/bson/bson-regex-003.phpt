--TEST--
MongoDB\BSON\Regex with flags omitted
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$regexp = new MongoDB\BSON\Regex("regexp");
printf("Pattern: %s\n", $regexp->getPattern());
printf("Flags: %s\n", $regexp->getFlags());
printf("String representation: %s\n", $regexp);

$tests = array(
    array("regex" => $regexp),
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
Pattern: regexp
Flags: 
String representation: /regexp/
Test#0 { "regex" : { "$regex" : "regexp", "$options" : "" } }
string(54) "{ "regex" : { "$regex" : "regexp", "$options" : "" } }"
string(54) "{ "regex" : { "$regex" : "regexp", "$options" : "" } }"
bool(true)
===DONE===
