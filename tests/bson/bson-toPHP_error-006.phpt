--TEST--
MongoDB\BSON\toPHP(): BSON decoding exception with unknown BSON type
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$bson = fromPHP(["hello" => ["cruel" => "world"]]);
$bson[15] = chr(0x42);

echo throws(function() use ($bson) {
    toPHP($bson);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected unknown BSON type 0x42 for field path "hello.cruel". Are you using the latest driver?
===DONE===
