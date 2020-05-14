--TEST--
PHPC-1598: WriteConcern get_gc should not invoke get_properties
--FILE--
<?php

/* Note: the segfault only manifests if the get_properties handler reports an
 * allocated value (e.g. string instead of integer for "w" field). */
$wc = new MongoDB\Driver\WriteConcern('string');

$a = (object) ['wc' => $wc];
$b = (object) ['wc' => $wc];

$a->b = $b;
$b->a = $a;

printf("Collected cycles: %d\n", gc_collect_cycles());

unset($a, $b);

printf("Collected cycles: %d\n", gc_collect_cycles());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Collected cycles: 0
Collected cycles: 2
===DONE===
