--TEST--
MongoDB\BSON\fromPHP(): Encoding non-Persistable objects as a document field value
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

class MyDocument {
    private $foo = 1;
    protected $bar = 2;
    public $baz = 3;
}

$tests = array(
    array(new MongoDB\BSON\UTCDateTime(new MongoDB\BSON\Int64('1416445411987'))),
    array('x' => new MongoDB\BSON\UTCDateTime(new MongoDB\BSON\Int64('1416445411987'))),
    array(new MyDocument),
    array('x' => new MyDocument),
);

foreach ($tests as $document) {
    $s = MongoDB\BSON\fromPHP($document);
    echo "Test " . MongoDB\BSON\toJSON($s) . "\n";
    hex_dump($s);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toJSON() is deprecated in %s
Test { "0" : { "$date" : 1416445411987 } }
     0 : 10 00 00 00 09 30 00 93 c2 b9 ca 49 01 00 00 00  [.....0.....I....]

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toJSON() is deprecated in %s
Test { "x" : { "$date" : 1416445411987 } }
     0 : 10 00 00 00 09 78 00 93 c2 b9 ca 49 01 00 00 00  [.....x.....I....]

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toJSON() is deprecated in %s
Test { "0" : { "baz" : 3 } }
     0 : 16 00 00 00 03 30 00 0e 00 00 00 10 62 61 7a 00  [.....0......baz.]
    10 : 03 00 00 00 00 00                                [......]

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s

Deprecated: Function MongoDB\BSON\toJSON() is deprecated in %s
Test { "x" : { "baz" : 3 } }
     0 : 16 00 00 00 03 78 00 0e 00 00 00 10 62 61 7a 00  [.....x......baz.]
    10 : 03 00 00 00 00 00                                [......]
===DONE===
