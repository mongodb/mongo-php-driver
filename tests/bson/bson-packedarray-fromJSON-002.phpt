--TEST--
MongoDB\BSON\PackedArray::fromJSON(): Decoding extended JSON types
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    '[{ "$oid": "56315a7c6118fd1b920270b1" }]',
    '[{ "$binary": "Zm9v", "$type": "00" }]',
    '[{ "$date": "2015-10-28T00:00:00Z" }]',
    '[{ "$timestamp": { "t": 1446084619, "i": 0 }}]',
    '[{ "$regex": "pattern", "$options": "i" }]',
    '[{ "$undefined": true }]',
    '[{ "$minKey": 1 }]',
    '[{ "$maxKey": 1 }]',
    '[{ "$numberLong": "1234" }]',
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
Test [{ "$oid": "56315a7c6118fd1b920270b1" }]
     0 : 14 00 00 00 07 30 00 56 31 5a 7c 61 18 fd 1b 92  [.....0.V1Z|a....]
    10 : 02 70 b1 00                                      [.p..]
Test [{ "$binary": "Zm9v", "$type": "00" }]
     0 : 10 00 00 00 05 30 00 03 00 00 00 00 66 6f 6f 00  [.....0......foo.]
Test [{ "$date": "2015-10-28T00:00:00Z" }]
     0 : 10 00 00 00 09 30 00 00 80 be ab 50 01 00 00 00  [.....0.....P....]
Test [{ "$timestamp": { "t": 1446084619, "i": 0 }}]
     0 : 10 00 00 00 11 30 00 00 00 00 00 0b 80 31 56 00  [.....0.......1V.]
Test [{ "$regex": "pattern", "$options": "i" }]
     0 : 12 00 00 00 0b 30 00 70 61 74 74 65 72 6e 00 69  [.....0.pattern.i]
    10 : 00 00                                            [..]
Test [{ "$undefined": true }]
     0 : 08 00 00 00 06 30 00 00                          [.....0..]
Test [{ "$minKey": 1 }]
     0 : 08 00 00 00 ff 30 00 00                          [.....0..]
Test [{ "$maxKey": 1 }]
     0 : 08 00 00 00 7f 30 00 00                          [.....0..]
Test [{ "$numberLong": "1234" }]
     0 : 10 00 00 00 12 30 00 d2 04 00 00 00 00 00 00 00  [.....0..........]
===DONE===
