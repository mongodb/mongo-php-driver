--TEST--
Atlas Connectivity Tests
--SKIPIF--
<?php require __DIR__ . "/utils/basic-skipif.inc"; ?>
<?php if (getenv('TESTS') !== 'tests/atlas.phpt') { die('skip Atlas tests not wanted'); } ?>
--FILE--
<?php
require_once __DIR__ . "/utils/basic.inc";

$envs = [
	'ATLAS_SERVERLESS',
	'ATLAS_SRV_SERVERLESS',
	'ATLAS_FREE',
	'ATLAS_SRV_FREE',
	'ATLAS_REPL',
	'ATLAS_SRV_REPL',
	'ATLAS_SHRD',
	'ATLAS_SRV_SHRD',
	'ATLAS_TLS11',
	'ATLAS_SRV_TLS11',
	'ATLAS_TLS12',
	'ATLAS_SRV_TLS12',
];

$command = new \MongoDB\Driver\Command(['ping' => 1]);
$query = new \MongoDB\Driver\Query([]);

foreach ($envs as $env) {
	echo $env, ': ';
	$uri = getenv($env);

	if (! is_string($uri)) {
		echo "FAIL: env var is undefined\n";
		continue;
	}

	try {
		$m = new \MongoDB\Driver\Manager($uri);
		$m->executeCommand('admin', $command);
		iterator_to_array($m->executeQuery('test.test', $query));
		echo "PASS\n";
	} catch(Exception $e) {
		echo "FAIL: ", $e->getMessage(), "\n";
	}
}
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
ATLAS_SERVERLESS: PASS
ATLAS_SRV_SERVERLESS: PASS
ATLAS_FREE: PASS
ATLAS_SRV_FREE: PASS
ATLAS_REPL: PASS
ATLAS_SRV_REPL: PASS
ATLAS_SHRD: PASS
ATLAS_SRV_SHRD: PASS
ATLAS_TLS11: PASS
ATLAS_SRV_TLS11: PASS
ATLAS_TLS12: PASS
ATLAS_SRV_TLS12: PASS
===DONE===
