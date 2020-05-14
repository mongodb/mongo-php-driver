--TEST--
PHPC-1598: ReadPreference get_gc should not invoke get_properties
--FILE--
<?php

$rp = new MongoDB\Driver\ReadPreference('primary');

$a = (object) ['rp' => $rp];
$b = (object) ['rp' => $rp];

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
