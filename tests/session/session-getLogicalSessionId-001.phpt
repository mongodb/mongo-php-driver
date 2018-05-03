--TEST--
MongoDB\Driver\Session::getLogicalSessionId()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '3.6'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
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
