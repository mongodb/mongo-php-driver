--TEST--
BSON\fromPHP(): Encoding non-Persistable objects as a document field value
--INI--
date.timezone=America/Los_Angeles
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
use MongoDB\BSON as BSON;

require_once __DIR__ . "/../utils/basic.inc";

class MyDocument {
    private $foo = 1;
    protected $bar = 2;
    public $baz = 3;
}

$tests = array(
    array(new BSON\UTCDateTime('1416445411987')),
    array('x' => new BSON\UTCDateTime('1416445411987')),
    array(new MyDocument),
    array('x' => new MyDocument),
);

foreach ($tests as $document) {
    $s = fromPHP($document);
    echo "Test ", toJSON($s), "\n";
    hex_dump($s);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Test { "0" : { "$date" : 1416445411987 } }
     0 : 10 00 00 00 09 30 00 93 c2 b9 ca 49 01 00 00 00  [.....0.....I....]
Test { "x" : { "$date" : 1416445411987 } }
     0 : 10 00 00 00 09 78 00 93 c2 b9 ca 49 01 00 00 00  [.....x.....I....]
Test { "0" : { "foo" : 1, "bar" : 2, "baz" : 3 } }
     0 : 28 00 00 00 03 30 00 20 00 00 00 10 66 6f 6f 00  [(....0. ....foo.]
    10 : 01 00 00 00 10 62 61 72 00 02 00 00 00 10 62 61  [.....bar......ba]
    20 : 7a 00 03 00 00 00 00 00                          [z.......]
Test { "x" : { "foo" : 1, "bar" : 2, "baz" : 3 } }
     0 : 28 00 00 00 03 78 00 20 00 00 00 10 66 6f 6f 00  [(....x. ....foo.]
    10 : 01 00 00 00 10 62 61 72 00 02 00 00 00 10 62 61  [.....bar......ba]
    20 : 7a 00 03 00 00 00 00 00                          [z.......]
===DONE===
