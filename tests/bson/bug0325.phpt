--TEST--
Test for PHPC-325: Memory leak decoding buffers with multiple documents
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$bson1 = fromJSON('{"x": "y"}');
$bson2 = fromJSON('{"a": "b"}');
try {
    var_dump(toPHP($bson1 . $bson2));
} catch (MongoDB\Driver\Exception\UnexpectedValueException $e) {
    echo $e->getMessage(), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Reading document did not exhaust input buffer
===DONE===
