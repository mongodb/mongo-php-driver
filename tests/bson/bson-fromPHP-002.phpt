--TEST--
BSON\fromPHP(): Encoding non-Persistable objects as a document
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

$s = fromPHP(new MyDocument);
echo "Test ", toJSON($s), "\n";
hex_dump($s);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Test { "foo" : 1, "bar" : 2, "baz" : 3 }
     0 : 20 00 00 00 10 66 6f 6f 00 01 00 00 00 10 62 61  [ ....foo......ba]
    10 : 72 00 02 00 00 00 10 62 61 7a 00 03 00 00 00 00  [r......baz......]
===DONE===
