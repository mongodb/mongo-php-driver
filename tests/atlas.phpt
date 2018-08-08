--TEST--
Atlas Connectivity Tests
--SKIPIF--
<?php
if (!file_exists('.travis.scripts/atlas-uris.txt')) { echo "skip Atlas URIs not found\n"; }
if (filesize('.travis.scripts/atlas-uris.txt') < 10) { echo "skip Atlas URI file empty\n"; }
if ($_ENV['TESTS'] !== 'tests/atlas.phpt') { echo "skip Atlas tests not wanted\n"; }
?>
--FILE--
<?php
$urls = explode("\n", file_get_contents('.travis.scripts/atlas-uris.txt'));

$isMasterCmd = new \MongoDB\Driver\Command(['isMaster' => 1]);
$query = new \MongoDB\Driver\Query([]);

foreach ($urls as $url) {
	$url = trim($url);
	if ($url == '') {
		continue;
	}

	try {
		$m = new \MongoDB\Driver\Manager($url);
		$m->executeCommand('admin', $isMasterCmd);
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
PASS
PASS
PASS
PASS
PASS
FAIL: %s
===DONE===
