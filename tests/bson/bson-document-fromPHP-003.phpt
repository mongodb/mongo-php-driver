--TEST--
MongoDB\BSON\Document::fromPHP(): Encodes Persistable objects as a document
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

class MyDocument implements MongoDB\BSON\Persistable {
    private $foo = 1;
    protected $bar = 2;
    public $baz = 3;

    public function bsonSerialize(): object
    {
        return (object) [
            'foo' => $this->foo,
            'bar' => $this->bar,
            'baz' => $this->baz,
        ];
    }

    public function bsonUnserialize(array $data): void
    {
        // Unused
    }
}

$bson = MongoDB\BSON\Document::fromPHP(new MyDocument);
echo $bson->toRelaxedExtendedJSON(), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
{ "__pclass" : { "$binary" : { "base64" : "TXlEb2N1bWVudA==", "subType" : "80" } }, "foo" : 1, "bar" : 2, "baz" : 3 }
===DONE===
