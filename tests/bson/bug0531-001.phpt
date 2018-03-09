--TEST--
PHPC-531: Segfault due to double free by corrupt BSON visitor (top-level)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$bson = fromPHP(["hello" => "world"]);
$bson[4] = 1;

echo throws(function() use ($bson) {
    toPHP($bson);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected unknown BSON type 0x31 for field path "hello". Are you using the latest driver?
===DONE===
