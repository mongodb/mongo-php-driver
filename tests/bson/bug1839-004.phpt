--TEST--
PHPC-1839: Referenced, local, interned string in typeMap
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$rootValue = 'array';
$documentValue = 'array';

$typemap = ['root' => &$rootValue, 'document' => &$documentValue];
$bson    = MongoDB\BSON\fromPhp((object) []);

echo "Before:\n";
debug_zval_dump($typemap);

MongoDB\BSON\toPhp($bson, $typemap);

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
