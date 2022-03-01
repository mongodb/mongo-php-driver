--TEST--
Causal consistency: new session has no operation time
--DESCRIPTION--
Causal consistency spec prose test #1
https://github.com/mongodb/specifications/blob/master/source/causal-consistency/causal-consistency.rst#test-plan
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_replica_set_or_sharded_cluster_with_replica_set(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$session = $manager->startSession();

echo "Initial operation time:\n";
var_dump($session->getOperationTime());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Initial operation time:
NULL
===DONE===
