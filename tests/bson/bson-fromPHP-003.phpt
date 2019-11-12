--TEST--
MongoDB\BSON\fromPHP(): Encoding non-Persistable objects as a document field value
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class MyDocument {
    private $foo = 1;
    protected $bar = 2;
    public $baz = 3;
}

$tests = array(
    array(new MongoDB\BSON\UTCDateTime('1416445411987')),
    array('x' => new MongoDB\BSON\UTCDateTime('1416445411987')),
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
Test { "0" : { "baz" : 3 } }
     0 : 16 00 00 00 03 30 00 0e 00 00 00 10 62 61 7a 00  [.....0......baz.]
    10 : 03 00 00 00 00 00                                [......]
Test { "x" : { "baz" : 3 } }
     0 : 16 00 00 00 03 78 00 0e 00 00 00 10 62 61 7a 00  [.....x......baz.]
    10 : 03 00 00 00 00 00                                [......]
===DONE===
