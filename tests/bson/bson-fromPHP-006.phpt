--TEST--
MongoDB\BSON\fromPHP(): PHP documents with null bytes in field name
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* Per PHPC-884, field names with a leading null byte are ignored when encoding
 * a document from an object's property hash table, since PHP uses leading bytes
 * to denote protected and private properties. */
echo "\nTesting object with one leading null byte in field name\n";
hex_dump(MongoDB\BSON\fromPHP((object) ["\0" => 1]));

echo "\nTesting object with multiple null bytes in field name\n";
hex_dump(MongoDB\BSON\fromPHP((object) ["\0\0\0" => 1]));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing object with one leading null byte in field name

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
     0 : 05 00 00 00 00                                   [.....]

Testing object with multiple null bytes in field name

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
     0 : 05 00 00 00 00                                   [.....]
===DONE===
