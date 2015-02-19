--TEST--
Connect to MongoDB with using SSL and X.509 auth
--SKIPIF--
<?php require "tests/utils/auth-x509-skipif.inc"?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$SSL_DIR = realpath(__DIR__ . "/" . "./../../scripts/ssl/");

$opts = array(
    "ssl" => array(
        "peer_name" => "MongoDB",
        "verify_peer" => true,
        "verify_peer_name" => true,
        "allow_self_signed" => false,
        "cafile" => $SSL_DIR . "/ca.pem", /* Defaults to openssl.cafile */
        "capath" => $SSL_DIR, /* Defaults to openssl.capath */
        "local_cert" => $SSL_DIR . "/client.pem",
        "passphrase" => "qwerty",
        "CN_match" => "server",
        "verify_depth" => 5,
        "ciphers" => "HIGH:!EXPORT:!aNULL@STRENGTH",
        "capture_peer_cert" => true,
        "capture_peer_cert_chain" => true,
        "SNI_enabled" => true,
        "disable_compression" => false,
        "peer_fingerprint" => "0d6dbd95",
    ),
);
$context = stream_context_create($opts);

$certusername = "C=US,ST=New York,L=New York City,O=MongoDB,OU=KernelUser,CN=client";


try {
    $parsed = parse_url(MONGODB_STANDALONE_X509_URI);
    $dsn = sprintf("mongodb://%s@%s:%d/%s?ssl=true&authMechanism=MONGODB-X509", $certusername, $parsed["host"], $parsed["port"], DATABASE_NAME);

    $manager = new MongoDB\Driver\Manager($dsn, array(), array("context" => $context));

    $batch = new MongoDB\Driver\WriteBatch();
    $batch->insert(array("very" => "important"));
    $manager->executeWriteBatch(NS, $batch);
    $query = new MongoDB\Driver\Query(array("very" => "important"));
    $cursor = $manager->executeQuery(NS, $query);
    foreach($cursor as $document) {
        var_dump($document["very"]);
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
