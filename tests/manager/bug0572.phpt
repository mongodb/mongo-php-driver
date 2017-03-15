--TEST--
PHPC-572: Ensure stream context does not go out of scope before socket init
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('STANDALONE_SSL'); ?>
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
    return new MongoDB\Driver\Manager(STANDALONE_SSL, ['ssl' => true], ['context' => $context]);
};

$manager = $closure();
$cursor = $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));
var_dump($cursor->toArray()[0]);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["ok"]=>
  float(1)
}
===DONE===
