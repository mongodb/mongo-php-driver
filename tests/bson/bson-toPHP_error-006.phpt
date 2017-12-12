--TEST--
MongoDB\BSON\toPHP(): BSON decoding shows multiple warnings
--FILE--
<?php

$tests = [
    // two undefined fields in root document
    pack('VCa*xCa*xx', 13, 0x06, 'u1', 0x06, 'u2'),
    // undefined field and symbol field in root document
    pack('VCa*xCa*xVa*xx', 21, 0x06, 'u1', 0x0E, 's1', 4, 'foo'),
    // two undefined fields in root (first) and embedded (second) documents
    pack('VCa*xCa*xVCa*xxx', 22, 0x06, 'u1', 0x03, 'e1', 9, 0x06, 'u2'),
    // two undefined fields in embedded (first) and root (second) documents
    pack('VCa*xVCa*xxCa*xx', 22, 0x03, 'e1', 9, 0x06, 'u1', 0x06, 'u2'),
    // two undefined fields in separate embedded documents
    pack('VCa*xVCa*xxCa*xVCa*xxx', 31, 0x03, 'e1', 9, 0x06, 'u1', 0x03, 'e2', 9, 0x06, 'u2'),
];

ini_set('mongodb.debug', 'stdout');
foreach ($tests as $bson) {
    var_dump(MongoDB\BSON\toPHP($bson));
}
ini_set('mongodb.debug', 'off');
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
[%s] PHONGO-BSON: WARNING > Detected unsupported BSON type 0x06 (undefined) for fieldname "u1"
[%s] PHONGO-BSON: WARNING > Detected unsupported BSON type 0x06 (undefined) for fieldname "u2"
object(stdClass)#%d (%d) {
}
[%s] PHONGO-BSON: WARNING > Detected unsupported BSON type 0x06 (undefined) for fieldname "u1"
[%s] PHONGO-BSON: WARNING > Detected unsupported BSON type 0x0E (symbol) for fieldname "s1"
object(stdClass)#%d (%d) {
}
[%s] PHONGO-BSON: WARNING > Detected unsupported BSON type 0x06 (undefined) for fieldname "u1"
[%s] PHONGO-BSON: WARNING > Detected unsupported BSON type 0x06 (undefined) for fieldname "u2"
object(stdClass)#%d (%d) {
  ["e1"]=>
  object(stdClass)#%d (0) {
  }
}
[%s] PHONGO-BSON: WARNING > Detected unsupported BSON type 0x06 (undefined) for fieldname "u1"
[%s] PHONGO-BSON: WARNING > Detected unsupported BSON type 0x06 (undefined) for fieldname "u2"
object(stdClass)#%d (%d) {
  ["e1"]=>
  object(stdClass)#%d (0) {
  }
}
[%s] PHONGO-BSON: WARNING > Detected unsupported BSON type 0x06 (undefined) for fieldname "u1"
[%s] PHONGO-BSON: WARNING > Detected unsupported BSON type 0x06 (undefined) for fieldname "u2"
object(stdClass)#%d (%d) {
  ["e1"]=>
  object(stdClass)#%d (0) {
  }
  ["e2"]=>
  object(stdClass)#%d (0) {
  }
}
===DONE===
