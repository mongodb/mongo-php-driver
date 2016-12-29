--TEST--
BSON\fromPHP(): PHP document with public property whose name is an empty string
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    ['' => 1],
    (object) ['' => 1],
];

foreach ($tests as $document) {
    $s = fromPHP($document);
    echo "Test ", toJSON($s), "\n";
    hex_dump($s);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Test { "" : 1 }
     0 : 0b 00 00 00 10 00 01 00 00 00 00                 [...........]
Test { "" : 1 }
     0 : 0b 00 00 00 10 00 01 00 00 00 00                 [...........]
===DONE===
