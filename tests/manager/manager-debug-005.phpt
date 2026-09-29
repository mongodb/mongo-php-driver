--TEST--
PHPC-1519: MongoDB\Driver\Manager debug output includes Server objects in cluster
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$manager->selectServer(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY));

var_dump($manager);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\Manager)#%d (%d) {
  ["uri"]=>
  string(%d) "%s"
  ["cluster"]=>
  array(%d) {
    [%d]=>
    object(MongoDB\Driver\Server)#%d (%d) {
      %A
    }%A
  }
  ["cryptSharedVersion"]=>
  NULL
}
===DONE===
