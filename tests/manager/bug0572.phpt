--TEST--
PHPC-572: Ensure stream context does not go out of scope before socket init
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_ssl(); ?>
<?php skip_if_not_ssl(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$closure = function() {
    $context = stream_context_create([
        'ssl' => [
            'verify_peer' => false,
            'verify_peer_name' => false,
            'allow_self_signed' => true,
        ],
    ]);
    return new MongoDB\Driver\Manager(URI, [], ['context' => $context]);
};

$manager = $closure();
$cursor = $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));
var_dump($cursor->toArray()[0]);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: MongoDB\Driver\Manager::__construct(): The "context" driver option is deprecated.%s

Deprecated: MongoDB\Driver\Manager::__construct(): The "allow_self_signed" context driver option is deprecated. Please use the "tlsAllowInvalidCertificates" URI option instead.%s
object(stdClass)#%d (%d) {
  ["ok"]=>
  float(1)
}
===DONE===
