--TEST--
PHPC-1839: Referenced, local, non-interned string in typeMap (PHP < 8.1)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '8.1'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

// Assemble the string so as to not have an interned string
$rootValue = chr(ord('a')) . 'rray';
$documentValue = chr(ord('a')) . 'rray';

$typemap = ['root' => &$rootValue, 'document' => &$documentValue];
$bson    = MongoDB\BSON\fromPhp((object) []);

echo "Before:\n";
debug_zval_dump($typemap);

MongoDB\BSON\toPHP($bson, $typemap);

echo "After:\n";
debug_zval_dump($typemap);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Before:
array(2) refcount(2){
  ["root"]=>
  &string(5) "array" refcount(1)
  ["document"]=>
  &string(5) "array" refcount(1)
}
After:
array(2) refcount(2){
  ["root"]=>
  &string(5) "array" refcount(1)
  ["document"]=>
  &string(5) "array" refcount(1)
}
===DONE===
