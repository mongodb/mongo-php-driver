--TEST--
X509 connection should not reuse previous stream after an auth failure
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; NEEDS("STANDALONE_X509"); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

function connect($dsn, $opts) {
    try {
        $manager = new MongoDB\Driver\Manager($dsn, array(), $opts);

        $bulk = new MongoDB\Driver\BulkWrite();
        $bulk->insert(array("very" => "important"));
        $manager->executeBulkWrite(NS, $bulk);
        echo "Connected\n";
    } catch(Exception $e) {
        echo get_class($e), ": ", $e->getMessage(), "\n";
    }
    return $manager;
}

$SSL_DIR = realpath(__DIR__ . "/" . "./../../scripts/ssl/");
$opts = array(
        "peer_name" => "server",
        "verify_peer" => true,
        "verify_peer_name" => true,
        "allow_self_signed" => false,
        "cafile" => $SSL_DIR . "/ca.pem", /* Defaults to openssl.cafile */
        "capath" => $SSL_DIR, /* Defaults to openssl.capath */
        "local_cert" => $SSL_DIR . "/client.pem",
);

/* Wrong username */
$parsed = parse_url(STANDALONE_X509);
$dsn = sprintf("mongodb://username@%s:%d/%s?ssl=true&authMechanism=MONGODB-X509", $parsed["host"], $parsed["port"], DATABASE_NAME);

$m1 = connect($dsn, $opts);
$m2 = connect($dsn, $opts);

echo "Both should have failed with auth failure - without reusing previous stream\n";
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
MongoDB\Driver\Exception\AuthenticationException: auth failed
MongoDB\Driver\Exception\AuthenticationException: auth failed
Both should have failed with auth failure - without reusing previous stream
===DONE===
