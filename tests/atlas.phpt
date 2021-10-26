--TEST--
Atlas Connectivity Tests
--SKIPIF--
<?php
if (!file_exists('.evergreen/atlas-uris.txt')) { echo "skip Atlas URIs not found\n"; }
if (filesize('.evergreen/atlas-uris.txt') < 10) { echo "skip Atlas URI file empty\n"; }
if (getenv('TESTS') !== 'tests/atlas.phpt') { echo "skip Atlas tests not wanted\n"; }
?>
--FILE--
<?php
$urls = explode("\n", file_get_contents('.evergreen/atlas-uris.txt'));

$command = new \MongoDB\Driver\Command(['ping' => 1]);
$query = new \MongoDB\Driver\Query([]);

foreach ($urls as $url) {
	$url = trim($url);

	if ($url == '') {
		continue;
	}

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
