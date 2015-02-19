--TEST--
Connect to MongoDB with using SSL and verify the stream
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$dsn = sprintf("%s/?ssl=true", MONGODB_STANDALONE_SSL_URI);

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
        "peer_name" => "MongoDB",
        "verify_peer" => true,
        "verify_peer_name" => true,
        "allow_self_signed" => false,
        "cafile" => $SSL_DIR . "/ca.pem", /* Defaults to openssl.cafile */
        "capath" => $SSL_DIR, /* Defaults to openssl.capath */
        "local_cert" => $SSL_DIR . "/client.pem",
        "passphrase" => "Very secretive client.pem passphrase",
        "CN_match" => "Common Name (CN) match",
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

$mc = new MongoDB\Driver\Manager($dsn, array(), array("context" => $context));


echo throws(function() use($mc) {
    $batch = new MongoDB\Driver\WriteBatch;
    $batch->insert(array("my" => "value"));
    $retval = $mc->executeWriteBatch(NS, $batch);
}, "MongoDB\\Driver\\SSLConnectionException", "executeWriteBatch"), "\n";


echo "Changing to server\n";
stream_context_set_option($context, "ssl", "CN_match", "server");
$batch = new MongoDB\Driver\WriteBatch;
$batch->insert(array("my" => "value"));
$retval = $mc->executeWriteBatch(NS, $batch);
printf("Inserted: %d\n", $retval->getInsertedCount());



$opts = stream_context_get_params($context);
$cert = openssl_x509_parse($opts["options"]["ssl"]["peer_certificate"]);
printf("Certificate name: %s\n", $cert["name"]);
printf("Certificate valid (not expired): %s\n", isValid($cert) ? "OK" : "NO");
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\SSLConnectionException thrown from executeWriteBatch
%s
Changing to server
Inserted: 1
Certificate name: /CN=server/OU=Kernel/O=MongoDB/L=New York City/ST=New York/C=US
Certificate valid (not expired): NO
===DONE===
