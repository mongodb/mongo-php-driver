--TEST--
PHPC-1839: Non-referenced interned string in typeMap
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$rootValue = 'array';
$documentValue = 'array';

$typemap = ['root' => &$rootValue, 'document' => &$documentValue];

$manager = new MongoDB\Driver\Manager(URI);
$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));

echo "Before:\n";
debug_zval_dump($typemap);

$cursor->setTypemap($typemap);

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
