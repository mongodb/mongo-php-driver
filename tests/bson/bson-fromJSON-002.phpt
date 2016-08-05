--TEST--
MongoDB\BSON\fromJSON(): Decoding extended JSON types
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    '{ "_id": { "$oid": "56315a7c6118fd1b920270b1" }}',
    '{ "binary": { "$binary": "Zm9v", "$type": "00" }}',
    '{ "date": { "$date": "2015-10-28T00:00:00Z" }}',
    '{ "timestamp": { "$timestamp": { "t": 1446084619, "i": 0 }}}',
    '{ "regex": { "$regex": "pattern", "$options": "i" }}',
    '{ "undef": { "$undefined": true }}',
    '{ "minkey": { "$minKey": 1 }}',
    '{ "maxkey": { "$maxKey": 1 }}',
    '{ "long": { "$numberLong": "1234" }}',
];

foreach ($tests as $json) {
    printf("Test %s\n", $json);
    $bson = fromJSON($json);
    hex_dump($bson);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Test { "_id": { "$oid": "56315a7c6118fd1b920270b1" }}
     0 : 16 00 00 00 07 5f 69 64 00 56 31 5a 7c 61 18 fd  [....._id.V1Z|a..]
    10 : 1b 92 02 70 b1 00                                [...p..]
Test { "binary": { "$binary": "Zm9v", "$type": "00" }}
     0 : 15 00 00 00 05 62 69 6e 61 72 79 00 03 00 00 00  [.....binary.....]
    10 : 00 66 6f 6f 00                                   [.foo.]
Test { "date": { "$date": "2015-10-28T00:00:00Z" }}
     0 : 13 00 00 00 09 64 61 74 65 00 00 80 be ab 50 01  [.....date.....P.]
    10 : 00 00 00                                         [...]
Test { "timestamp": { "$timestamp": { "t": 1446084619, "i": 0 }}}
     0 : 18 00 00 00 11 74 69 6d 65 73 74 61 6d 70 00 00  [.....timestamp..]
    10 : 00 00 00 0b 80 31 56 00                          [.....1V.]
Test { "regex": { "$regex": "pattern", "$options": "i" }}
     0 : 16 00 00 00 0b 72 65 67 65 78 00 70 61 74 74 65  [.....regex.patte]
    10 : 72 6e 00 69 00 00                                [rn.i..]
Test { "undef": { "$undefined": true }}
     0 : 0c 00 00 00 06 75 6e 64 65 66 00 00              [.....undef..]
Test { "minkey": { "$minKey": 1 }}
     0 : 0d 00 00 00 ff 6d 69 6e 6b 65 79 00 00           [.....minkey..]
Test { "maxkey": { "$maxKey": 1 }}
     0 : 0d 00 00 00 7f 6d 61 78 6b 65 79 00 00           [.....maxkey..]
Test { "long": { "$numberLong": "1234" }}
     0 : 13 00 00 00 12 6c 6f 6e 67 00 d2 04 00 00 00 00  [.....long.......]
    10 : 00 00 00                                         [...]
===DONE===
