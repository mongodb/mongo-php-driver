--TEST--
MongoDB\BSON\Document::fromJSON(): Decoding JSON
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    '{}',
    '{ "foo": "bar" }',
    '{ "foo": [ 1, 2, 3 ]}',
    '{ "foo": { "bar": 1 }}',
    '[ 1, 2, 3 ]', // This will be interpreted as an object, i.e. { "0": 1, "1": 2, "2": 3 }
];

foreach ($tests as $json) {
    printf("Test %s\n", $json);
    $bson = MongoDB\BSON\Document::fromJSON($json);
    hex_dump((string) $bson);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Test {}
     0 : 05 00 00 00 00                                   [.....]
Test { "foo": "bar" }
     0 : 12 00 00 00 02 66 6f 6f 00 04 00 00 00 62 61 72  [.....foo.....bar]
    10 : 00 00                                            [..]
Test { "foo": [ 1, 2, 3 ]}
     0 : 24 00 00 00 04 66 6f 6f 00 1a 00 00 00 10 30 00  [$....foo......0.]
    10 : 01 00 00 00 10 31 00 02 00 00 00 10 32 00 03 00  [.....1......2...]
    20 : 00 00 00 00                                      [....]
Test { "foo": { "bar": 1 }}
     0 : 18 00 00 00 03 66 6f 6f 00 0e 00 00 00 10 62 61  [.....foo......ba]
    10 : 72 00 01 00 00 00 00 00                          [r.......]
Test [ 1, 2, 3 ]
     0 : 1a 00 00 00 10 30 00 01 00 00 00 10 31 00 02 00  [.....0......1...]
    10 : 00 00 10 32 00 03 00 00 00 00                    [...2......]
===DONE===
