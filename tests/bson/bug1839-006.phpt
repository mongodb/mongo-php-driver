--TEST--
PHPC-1839: Referenced, local, non-interned string in typeMap (PHP >= 8.1)
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

// Assemble the string so as to not have an interned string
$rootValue = chr(ord('a')) . 'rray';
$documentValue = chr(ord('a')) . 'rray';

$typemap = ['root' => &$rootValue, 'document' => &$documentValue];
$bson    = MongoDB\BSON\Document::fromPHP((object) []);

echo "Before:\n";
debug_zval_dump($typemap);

$bson->toPHP($typemap);

echo "After:\n";
debug_zval_dump($typemap);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Before:
array(2) refcount(2){
  ["root"]=>
  reference refcount(2) {
    string(5) "array" refcount(1)
  }
  ["document"]=>
  reference refcount(2) {
    string(5) "array" refcount(1)
  }
}
After:
array(2) refcount(2){
  ["root"]=>
  reference refcount(2) {
    string(5) "array" refcount(1)
  }
  ["document"]=>
  reference refcount(2) {
    string(5) "array" refcount(1)
  }
}
===DONE===
