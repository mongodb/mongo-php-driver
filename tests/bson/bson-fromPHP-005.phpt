--TEST--
MongoDB\BSON\fromPHP(): PHP document with public property whose name is an empty string
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    ['' => 1],
    (object) ['' => 1],
];

foreach ($tests as $document) {
    $s = MongoDB\BSON\fromPHP($document);
    echo "Test " . MongoDB\BSON\toJSON($s) . "\n";
    hex_dump($s);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toJSON() is deprecated in %s
Test { "" : 1 }
     0 : 0b 00 00 00 10 00 01 00 00 00 00                 [...........]

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toJSON() is deprecated in %s
Test { "" : 1 }
     0 : 0b 00 00 00 10 00 01 00 00 00 00                 [...........]
===DONE===
