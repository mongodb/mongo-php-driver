--TEST--
Connect to MongoDB with using X509 retrieving username from certificate (SSL options)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; NEEDS("STANDALONE_X509"); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

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

try {
    /* mongoc will pull the username of the certificate */
    $parsed = parse_url(STANDALONE_X509);
    $dsn = sprintf("mongodb://%s:%d/%s?ssl=true&authMechanism=MONGODB-X509", $parsed["host"], $parsed["port"], DATABASE_NAME);

    $manager = new MongoDB\Driver\Manager($dsn, array(), $opts);

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
