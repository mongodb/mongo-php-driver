--TEST--
PHPC-357: The exception for "invalid namespace" does not list the broken name
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$m = new MongoDB\Driver\Manager(URI);
$c = new MongoDB\Driver\Query(array());

echo throws(function() use($m, $c) {
    $m->executeQuery( 'demo', $c );
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Invalid namespace provided: demo
===DONE===
