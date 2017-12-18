--TEST--
MongoDB\Driver\Session::getLogicalSessionId()
--SKIPIF--
<?php if (getenv("TRAVIS")) exit("skip This currently fails on Travis because it doesn't run 3.6 yet"); ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('STANDALONE'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);
$session = $manager->startSession();

var_dump($session->getLogicalSessionId());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["id"]=>
  object(MongoDB\BSON\Binary)#%d (%d) {
    ["data"]=>
    string(16) "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"
    ["type"]=>
    int(4)
  }
}
===DONE===
