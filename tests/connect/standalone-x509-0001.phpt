--TEST--
Connect to MongoDB with using SSL and verify the stream
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
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
        "passphrase" => "Very secretive client.pem passphrase",
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

$parsed = parse_url(MONGODB_STANDALONE_X509_URI);
$adminuser = "root";
$adminpass = "toor";
$dsn = sprintf("mongodb://%s:%s@%s:%d/admin?ssl=true", $adminuser, $adminpass, $parsed["host"], $parsed["port"]);
$adminmanager = new MongoDB\Manager($dsn, array(), array("context" => $context, "debug" => STDERR));

$certusername = "C=US,ST=New York,L=New York City,O=MongoDB,OU=KernelUser,CN=client";


$cmd = array(
    "createUser" => $certusername,
    "roles" => [["role" => "readWrite", "db" => DATABASE_NAME]],
);

try {
    echo "User Created\n";
    $command = new MongoDB\Command($cmd);
    $result = $adminmanager->executeCommand(DATABASE_NAME, $command);
    echo "User Created\n";
} catch(Exception $e) {
    echo get_class($e), ": ", $e->getMessage(), "\n";
}

try {
    $parsed = parse_url(MONGODB_STANDALONE_X509_URI);
    $dsn = sprintf("mongodb://%s@%s:%d/%s?ssl=true&authMechanism=MONGODB-X509", urlencode($certusername), $parsed["host"], $parsed["port"], DATABASE_NAME);

    $manager = new MongoDB\Manager($dsn, array(), array("context" => $context, "debug" => STDERR));

    $batch = new MongoDB\WriteBatch();
    $batch->insert(array("very" => "important"));
    $manager->executeWriteBatch(NS, $batch);
    $query = new MongoDB\Query(array("very" => "important"));
    $cursor = $manager->executeQuery(NS, $query);
    foreach($cursor as $document) {
        var_dump($document["very"]);
    }
} catch(Exception $e) {
    echo get_class($e), ": ", $e->getMessage(), "\n";
}

try {
    echo "User dropped\n";
    $command = new MongoDB\Command(array("drop" => COLLECTION_NAME));
    $result = $adminmanager->executeCommand(DATABASE_NAME, $command);
    echo "User dropped\n";
} catch(Exception $e) {
    echo get_class($e), ": ", $e->getMessage(), "\n";
}


?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
User Created
string(9) "important"
User dropped
===DONE===
