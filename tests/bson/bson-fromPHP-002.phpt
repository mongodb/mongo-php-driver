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
Test { "baz" : 3 }
     0 : 0e 00 00 00 10 62 61 7a 00 03 00 00 00 00        [.....baz......]
===DONE===
