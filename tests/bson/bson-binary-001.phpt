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
--EXPECTF--
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
Test#0 { "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "00" } } }
string(89) "{ "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "00" } } }"
string(89) "{ "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "00" } } }"
bool(true)
Test#1 { "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "01" } } }
string(89) "{ "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "01" } } }"
string(89) "{ "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "01" } } }"
bool(true)
Test#2 { "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "02" } } }
string(89) "{ "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "02" } } }"
string(89) "{ "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "02" } } }"
bool(true)
Test#3 { "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "03" } } }
string(89) "{ "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "03" } } }"
string(89) "{ "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "03" } } }"
bool(true)
Test#4 { "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "04" } } }
string(89) "{ "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "04" } } }"
string(89) "{ "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "04" } } }"
bool(true)
Test#5 { "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "05" } } }
string(89) "{ "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "05" } } }"
string(89) "{ "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "05" } } }"
bool(true)
Test#6 { "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "80" } } }
string(89) "{ "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "80" } } }"
string(89) "{ "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "80" } } }"
bool(true)
Test#7 { "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "85" } } }
string(89) "{ "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "85" } } }"
string(89) "{ "binary" : { "$binary" : { "base64": "cmFuZG9tQmluYXJ5RGF0YQ==", "subType" : "85" } } }"
bool(true)
===DONE===
