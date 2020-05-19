--TEST--
PHPC-1598: ReadConcern get_gc should not invoke get_properties
--FILE--
<?php

$rc = new MongoDB\Driver\ReadConcern('local');

$a = (object) ['rc' => $rc];
$b = (object) ['rc' => $rc];

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
