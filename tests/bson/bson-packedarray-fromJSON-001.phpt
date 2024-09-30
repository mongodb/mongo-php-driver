--TEST--
MongoDB\BSON\PackedArray::fromJSON(): Decoding JSON
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    '[]',
    '[ 1, 2, 3 ]',
    '[[ 1, 2, 3 ]]',
    '[{ "bar": 1 }]',
    // JSON objects are accepted iff the keys are valid
    '{ "0": "foo", "1": "bar" }',
];

foreach ($tests as $json) {
    printf("Test %s\n", $json);
    $bson = MongoDB\BSON\PackedArray::fromJSON($json);
    hex_dump((string) $bson);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Test []
     0 : 05 00 00 00 00                                   [.....]
Test [ 1, 2, 3 ]
     0 : 1a 00 00 00 10 30 00 01 00 00 00 10 31 00 02 00  [.....0......1...]
    10 : 00 00 10 32 00 03 00 00 00 00                    [...2......]
Test [[ 1, 2, 3 ]]
     0 : 22 00 00 00 04 30 00 1a 00 00 00 10 30 00 01 00  ["....0......0...]
    10 : 00 00 10 31 00 02 00 00 00 10 32 00 03 00 00 00  [...1......2.....]
    20 : 00 00                                            [..]
Test [{ "bar": 1 }]
     0 : 16 00 00 00 03 30 00 0e 00 00 00 10 62 61 72 00  [.....0......bar.]
    10 : 01 00 00 00 00 00                                [......]
Test { "0": "foo", "1": "bar" }
     0 : 1b 00 00 00 02 30 00 04 00 00 00 66 6f 6f 00 02  [.....0.....foo..]
    10 : 31 00 04 00 00 00 62 61 72 00 00                 [1.....bar..]
===DONE===
