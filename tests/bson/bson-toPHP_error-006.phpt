--TEST--
MongoDB\BSON\toPHP(): BSON decoding exception with unknown BSON type
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$bson = MongoDB\BSON\fromPHP(["hello" => ["cruel" => "world"]]);
$bson[15] = chr(0x42);

echo throws(function() use ($bson) {
    MongoDB\BSON\toPHP($bson);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected unknown BSON type 0x42 for field path "hello.cruel". Are you using the latest driver?
===DONE===
