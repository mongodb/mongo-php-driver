--TEST--
PHPC-1015: Initial DNS Seedlist test
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('REPLICASET_DNS'); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

// STANDALONE does not support auth, but that is not necessary for the test
$m = new MongoDB\Driver\Manager("mongodb+srv://test1.test.build.10gen.cc/");
$s = $m->selectServer( new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_NEAREST ) );
$servers = $m->getServers();

foreach ( $servers as $server )
{
	echo $server->getHost(), ':', $server->getPort(), "\n";
}
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%d.%d.%d.%d:27017
%d.%d.%d.%d:27018
%d.%d.%d.%d:27019
===DONE===
