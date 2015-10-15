--TEST--
Connect to MongoDB with using SSL and X.509 auth (stream context)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; NEEDS("STANDALONE_X509"); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$SSL_DIR = realpath(__DIR__ . "/" . "./../../scripts/ssl/");

$opts = array(
    "ssl" => array(
        "peer_name" => "server",
        "verify_peer" => true,
        "verify_peer_name" => true,
        "allow_self_signed" => false,
        "cafile" => $SSL_DIR . "/ca.pem", /* Defaults to openssl.cafile */
        "capath" => $SSL_DIR, /* Defaults to openssl.capath */
        "local_cert" => $SSL_DIR . "/client.pem",
        "passphrase" => "qwerty",
        "verify_depth" => 5,
        "ciphers" => "HIGH:!EXPORT:!aNULL@STRENGTH",
        "capture_peer_cert" => true,
        "capture_peer_cert_chain" => true,
        "SNI_enabled" => true,
        "disable_compression" => false,
        "peer_fingerprint" => strtolower("FC16D0861C31D29E90A8A5C832469AB10EE7F4DD"),
    ),
);
$context = stream_context_create($opts);

try {
    $dsn = sprintf("%s&ssl=true", STANDALONE_X509);

    $manager = new MongoDB\Driver\Manager($dsn, array(), array("context" => $context));

    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(array("very" => "important"));
    $manager->executeBulkWrite(NS, $bulk);
    $query = new MongoDB\Driver\Query(array("very" => "important"));
    $cursor = $manager->executeQuery(NS, $query);
    foreach($cursor as $document) {
        var_dump($document->very);
    }
    $command = new MongoDB\Driver\Command(array("drop" => COLLECTION_NAME));
    $result = $manager->executeCommand(DATABASE_NAME, $command);
} catch(Exception $e) {
    echo get_class($e), ": ", $e->getMessage(), "\n";
}


?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
string(9) "important"
===DONE===
