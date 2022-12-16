--TEST--
MongoDB\BSON\Document::fromPHP(): Encodes non-Serializable objects as a document
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

class MyDocument {
    private $foo = 1;
    protected $bar = 2;
    public $baz = 3;
}

$bson = MongoDB\BSON\Document::fromPHP(new MyDocument);
echo $bson->toRelaxedExtendedJSON(), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
{ "baz" : 3 }
===DONE===
