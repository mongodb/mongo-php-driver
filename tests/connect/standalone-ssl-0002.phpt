--TEST--
MongoDB\Write\Batch: #001 Variety Batch
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php 
require_once "tests/utils/basic.inc";

$dsn = sprintf("%s/?ssl=true", MONGODB_STANDALONE_SSL_URI);

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
        "CN_match" => "Common Name (CN) match",
        "verify_depth" => 5,
        "ciphers" => "HIGH:!EXPORT:!aNULL@STRENGTH",
        "capture_peer_cert" => true,
        "capture_peer_cert_chain" => true,
        "SNI_enabled" => true,
        "disable_compression" => false,
        "peer_fingerprint" => "",
    ),
);
$context = stream_context_create($opts);

$mc = new MongoDB\Manager($dsn, array(), array("context" => $context, "debug" => STDERR));


echo throws(function() use($mc) {
    $batch = new MongoDB\WriteBatch;
    $batch->insert(array("my" => "value"));
    $retval = $mc->executeWriteBatch(NS, $batch);
}, "MongoDB\\SSLConnectionException", "executeWriteBatch"), "\n";
/*

$opts = stream_context_get_params($context);
var_dump($opts);
$cert = openssl_x509_parse($opts["options"]["ssl"]["peer_certificate"]);
var_dump($cert["name"]);
 */
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\SSLConnectionException thrown from executeWriteBatch
%s
===DONE===
