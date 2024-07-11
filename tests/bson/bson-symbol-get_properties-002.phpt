--TEST--
MongoDB\BSON\Symbol get_properties handler (foreach)
--FILE--
<?php

$document = MongoDB\BSON\Document::fromJSON('{"symbol":{"$symbol":"test"}}')->toPHP();
$symbol = $document->symbol;

foreach ($symbol as $key => $value) {
    var_dump($key);
    var_dump($value);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(6) "symbol"
string(4) "test"
===DONE===
