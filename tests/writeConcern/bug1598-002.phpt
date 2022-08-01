--TEST--
PHPC-1598: WriteConcern get_gc should delegate to zend_std_get_properties
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '8.1.99'); ?>
--FILE--
<?php

/* Store an additional object reference as a public property on the
 * WriteConcern. This will leak if get_gc returns internally cached properties
 * (from our get_properties handler) instead of zend_std_get_properties.
 *
 * Note: we also use a string value for WriteConcern's "w" field to ensure its
 * internal properties have an allocated zval. */
$a = new stdClass;
$a->wc = new MongoDB\Driver\WriteConcern('string');
$a->wc->a = $a;

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
