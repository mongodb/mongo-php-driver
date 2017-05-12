--TEST--
MongoDB\BSON\toPHP(): Decodes Binary UUID types with any data length
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

/* Note: PHPC-895 requires Binary UUID types to have 16-byte data lengths during
 * construction and initialization from unserialize() and __set_state(), but the
 * ability to decoding existing BSON documents should be preserved. */
$tests = [
    pack('VCa*xVCa*x', 30, 0x05, 'foo', 15, 0x03, '0123456789abcde'), // UUID type 0x03 with 15 bytes
    pack('VCa*xVCa*x', 32, 0x05, 'foo', 17, 0x03, '0123456789abcdefg'), // UUID type 0x03 with 17 bytes
    pack('VCa*xVCa*x', 30, 0x05, 'foo', 15, 0x04, '0123456789abcde'), // UUID type 0x04 with 15 bytes
    pack('VCa*xVCa*x', 32, 0x05, 'foo', 17, 0x04, '0123456789abcdefg'), // UUID type 0x04 with 17 bytes
];

foreach ($tests as $bson) {
    var_dump(toPHP($bson));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["foo"]=>
  object(MongoDB\BSON\Binary)#%d (%d) {
    ["data"]=>
    string(15) "0123456789abcde"
    ["type"]=>
    int(3)
  }
}
object(stdClass)#%d (%d) {
  ["foo"]=>
  object(MongoDB\BSON\Binary)#%d (%d) {
    ["data"]=>
    string(17) "0123456789abcdefg"
    ["type"]=>
    int(3)
  }
}
object(stdClass)#%d (%d) {
  ["foo"]=>
  object(MongoDB\BSON\Binary)#%d (%d) {
    ["data"]=>
    string(15) "0123456789abcde"
    ["type"]=>
    int(4)
  }
}
object(stdClass)#%d (%d) {
  ["foo"]=>
  object(MongoDB\BSON\Binary)#%d (%d) {
    ["data"]=>
    string(17) "0123456789abcdefg"
    ["type"]=>
    int(4)
  }
}
===DONE===
