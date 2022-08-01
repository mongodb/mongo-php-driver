--TEST--
PHPC-1598: ReadConcern get_gc should delegate to zend_std_get_properties
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '8.1.99'); ?>
--FILE--
<?php

/* Store an additional object reference as a public property on the ReadConcern.
 * This will leak if get_gc returns internally cached properties (from our
 * get_properties handler) instead of zend_std_get_properties. */
$a = new stdClass;
$a->rc = new MongoDB\Driver\ReadConcern('local');
$a->rc->a = $a;

printf("Collected cycles: %d\n", gc_collect_cycles());

unset($a);

printf("Collected cycles: %d\n", gc_collect_cycles());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Collected cycles: 0
Collected cycles: 2
===DONE===
