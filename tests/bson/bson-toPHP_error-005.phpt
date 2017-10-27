--TEST--
MongoDB\BSON\toPHP(): BSON decoding exceptions for unsupported BSON types
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    pack('VCa*xx', 10, 0x06, 'foo'), // undefined
    pack('VCa*xVa*xx12x', 37, 0x0C, 'foo', 11, 'collection'), // DBPointer
    pack('VCa*xVa*xx', 18, 0x0E, 'foo', 4, 'bar'), // symbol
];

ini_set("mongodb.debug", "stdout");
foreach ($tests as $bson) {
	var_dump(toPHP($bson));
}
ini_set("mongodb.debug", "off");

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
[%s] PHONGO-BSON: WARNING > Detected unsupported BSON type 0x06 (undefined) for fieldname "foo"
object(stdClass)#%d (%d) {
  ["foo"]=>
  NULL
}
[%s] PHONGO-BSON: WARNING > Detected unsupported BSON type 0x0C (DBPointer) for fieldname "foo"
object(stdClass)#%d (%d) {
  ["foo"]=>
  NULL
}
[%s] PHONGO-BSON: WARNING > Detected unsupported BSON type 0x0E (symbol) for fieldname "foo"
object(stdClass)#%d (%d) {
  ["foo"]=>
  NULL
}
===DONE===
