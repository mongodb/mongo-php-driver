--TEST--
MongoDB\BSON\Binary #001
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$types = array(
    MongoDB\BSON\Binary::TYPE_GENERIC,
    MongoDB\BSON\Binary::TYPE_FUNCTION,
    MongoDB\BSON\Binary::TYPE_OLD_BINARY,
    MongoDB\BSON\Binary::TYPE_OLD_UUID,
    MongoDB\BSON\Binary::TYPE_UUID,
    MongoDB\BSON\Binary::TYPE_MD5,
    MongoDB\BSON\Binary::TYPE_ENCRYPTED,
    MongoDB\BSON\Binary::TYPE_USER_DEFINED,
    MongoDB\BSON\Binary::TYPE_USER_DEFINED+5,
);
$tests = array();
foreach($types as $type) {
    // Use 16-byte data to satisfy UUID requirements
    $binary = new MongoDB\BSON\Binary('randomBinaryData', $type);
    var_dump($binary->getData() === 'randomBinaryData');
    var_dump($binary->getType() == $type);
    $tests[] = array("binary" => $binary);
}

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
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
Test#0 { "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "00" } }
string(73) "{ "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "00" } }"
string(73) "{ "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "00" } }"
bool(true)
Test#1 { "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "01" } }
string(73) "{ "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "01" } }"
string(73) "{ "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "01" } }"
bool(true)
Test#2 { "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "02" } }
string(73) "{ "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "02" } }"
string(73) "{ "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "02" } }"
bool(true)
Test#3 { "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "03" } }
string(73) "{ "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "03" } }"
string(73) "{ "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "03" } }"
bool(true)
Test#4 { "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "04" } }
string(73) "{ "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "04" } }"
string(73) "{ "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "04" } }"
bool(true)
Test#5 { "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "05" } }
string(73) "{ "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "05" } }"
string(73) "{ "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "05" } }"
bool(true)
Test#6 { "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "06" } }
string(73) "{ "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "06" } }"
string(73) "{ "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "06" } }"
bool(true)
Test#7 { "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "80" } }
string(73) "{ "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "80" } }"
string(73) "{ "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "80" } }"
bool(true)
Test#8 { "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "85" } }
string(73) "{ "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "85" } }"
string(73) "{ "binary" : { "$binary" : "cmFuZG9tQmluYXJ5RGF0YQ==", "$type" : "85" } }"
bool(true)
===DONE===
