--TEST--
MongoDB\BSON\Document::fromBSON(): Decoding from BSON string
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$bson = hex2bin('1200000002666f6f00040000006261720000');
$document = MongoDB\BSON\Document::fromBSON($bson);
hex_dump((string) $document);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
     0 : 12 00 00 00 02 66 6f 6f 00 04 00 00 00 62 61 72  [.....foo.....bar]
    10 : 00 00                                            [..]
===DONE===
