--TEST--
Atlas Connectivity Tests
--SKIPIF--
<?php require __DIR__ . "/utils/basic-skipif.inc"; ?>
<?php if (getenv('TESTS') !== 'tests/atlas.phpt') { die('skip Atlas tests not wanted'); } ?>
--FILE--
<?php
require_once __DIR__ . "/utils/basic.inc";

function extractUriWithCertificate(string $env): ?array
{
	$uri = getenv($env);
	if (! is_string($uri)) {
		return null;
	}

	$cert = getenv($env . '_CERT_BASE64');
	if (! is_string($cert)) {
		return null;
	}

	$certPath = tempnam(sys_get_temp_dir(), 'cert_');
	$certContents = base64_decode($cert);
	if (! $certPath || ! $certContents) {
		return null;
	}

	file_put_contents($certPath, $certContents);
	chmod($certPath, 0600);

	return [
		'uri' => $uri . '&tlsCertificateKeyFile=' . $certPath,
		'certPath' => $certPath,
	];
}

function testConnection(string $uri): void
{
	try {
		$m = new \MongoDB\Driver\Manager($uri);
		$m->executeCommand(
			'admin',
			new \MongoDB\Driver\Command(['ping' => 1]),
		);
		iterator_to_array($m->executeQuery(
			'test.test',
			new \MongoDB\Driver\Query([]),
		));
		echo "PASS\n";
	} catch(Exception $e) {
		echo "FAIL: ", $e->getMessage(), "\n";
	}
}

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
$x509Envs = [
	'ATLAS_X509',
	'ATLAS_X509_DEV',
];

foreach ($envs as $env) {
	echo $env, ': ';
	$uri = getenv($env);

	if (! is_string($uri)) {
		echo "FAIL: env var is undefined\n";
		continue;
	}

	testConnection($uri);
}

foreach ($x509Envs as $env) {
	echo $env, ': ';
	$uriWithCertificate = extractUriWithCertificate($env);

	if (! is_array($uriWithCertificate)) {
		echo "FAIL: env var is undefined\n";
		continue;
	}

	['uri' => $uri, 'certPath' => $certPath] = $uriWithCertificate;

	try {
		testConnection($uri);
	} finally {
		@unlink($certPath);
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
ATLAS_X509: PASS
ATLAS_X509_DEV: PASS
===DONE===
