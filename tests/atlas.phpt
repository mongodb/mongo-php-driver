--TEST--
Atlas Connectivity Tests
--SKIPIF--
<?php require __DIR__ . "/utils/basic-skipif.inc"; ?>
<?php skip_if_no_atlas_connectivity_urls(); ?>
<?php if (getenv('TESTS') !== 'tests/atlas.phpt') { die('skip Atlas tests not wanted'); } ?>
--FILE--
<?php
require_once __DIR__ . "/utils/basic.inc";

$command = new \MongoDB\Driver\Command(['ping' => 1]);
$query = new \MongoDB\Driver\Query([]);

foreach (getAtlasConnectivityUrls() as $url) {
	if (strpos($url, '#') === 0) {
		echo trim(substr($url, 1)), "\n";
		continue;
	}

	try {
		$m = new \MongoDB\Driver\Manager($url);
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
Atlas Serverless
PASS
PASS
Atlas replica set (4.0)
PASS
PASS
Atlas sharded cluster (4.0)
PASS
PASS
Atlas free tier replica set
PASS
PASS
Atlas with only TLSv1.1 enabled (4.0)
PASS
PASS
Atlas with only TLSv1.2 enabled (4.0)
PASS
PASS
Atlas with only TLSv1.2 enabled (4.0) and bad credentials
FAIL: %s
FAIL: %s
===DONE===
