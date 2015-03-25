--TEST--
Connect to MongoDB with using SSL and verify the stream
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$dsn = sprintf("%s/?ssl=true", STANDALONE_SSL);

$SSL_DIR = realpath(__DIR__ . "/" . "./../../scripts/ssl/");
function isValid(array $cert) {
    $from = $cert["validFrom_time_t"];
    $to   = $cert["validTo_time_t"];

    $current = time();

    if ($from > $current && $current < $to) {
        return true;
    }

    return false;
}

$opts = array(
    "ssl" => array(
        "peer_name" => "WRONG PEER NAME",
        "verify_peer" => true,
        "verify_peer_name" => true,
        "allow_self_signed" => false,
        "cafile" => $SSL_DIR . "/ca.pem", /* Defaults to openssl.cafile */
        "capath" => $SSL_DIR, /* Defaults to openssl.capath */
        "local_cert" => $SSL_DIR . "/client.pem",
        "passphrase" => "Very secretive client.pem passphrase",
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

$manager = new MongoDB\Driver\Manager($dsn, array(), array("context" => $context));


echo throws(function() use($manager) {
    $bulk = new MongoDB\Driver\BulkWrite;
    $bulk->insert(array("my" => "value"));
    $retval = $manager->executeBulkWrite(NS, $bulk);
}, "MongoDB\\Driver\\SSLConnectionException", "executeBulkWrite"), "\n";


echo "Changing to server\n";
stream_context_set_option($context, "ssl", "peer_name", "server");
$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(array("my" => "value"));
$retval = $manager->executeBulkWrite(NS, $bulk);
printf("Inserted: %d\n", $retval->getInsertedCount());



$opts = stream_context_get_params($context);
$cert = openssl_x509_parse($opts["options"]["ssl"]["peer_certificate"]);
printf("Certificate name: %s\n", $cert["name"]);
printf("Certificate valid (not expired): %s\n", isValid($cert) ? "OK" : "NO");
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\SSLConnectionException thrown from executeBulkWrite
%s
Changing to server
Inserted: 1
Certificate name: /CN=server/OU=Kernel/O=MongoDB/L=New York City/ST=New York/C=US
Certificate valid (not expired): NO
===DONE===
