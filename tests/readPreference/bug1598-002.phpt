--TEST--
PHPC-1598: ReadPreference get_gc should delegate to zend_std_get_properties
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '8.2'); ?>
--FILE--
<?php

/* Store an additional object reference as a public property on the
 * ReadPreference. This will leak if get_gc returns internally cached properties
 * (from our get_properties handler) instead of zend_std_get_properties.
 *
 * Note: since ReadPreference uses typed properties, PHP stores dynamic
 * properties in a separate obj->properties HashTable. This HashTable is itself
 * a GC-collectable value, so 3 cycles are collected: the stdClass, the
 * ReadPreference object, and the dynamic properties HashTable. */
$a = new stdClass;
$a->rp = new MongoDB\Driver\ReadPreference('primary');
$a->rp->a = $a;

printf("Collected cycles: %d\n", gc_collect_cycles());

unset($a);

printf("Collected cycles: %d\n", gc_collect_cycles());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Collected cycles: 0
Collected cycles: 3
===DONE===
