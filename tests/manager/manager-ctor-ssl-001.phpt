--TEST--
MongoDB\Driver\Manager::__construct(): ssl option does not require driverOptions
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS_SSL(); ?>
--FILE--
<?php

/* Note: Since the STANDALONE_SSL server uses a self-signed certificate, we
 * cannot connect to it without also providing driver options. Since the purpose
 * of this test is to demonstrate that the SSL option does not require driver
 * options, we will simply dump the constructed Manager. */
var_dump(new MongoDB\Driver\Manager('mongodb://127.0.0.1/?ssl=true'));
var_dump(new MongoDB\Driver\Manager(null, ['ssl' => true]));

?>
===DONE===
--EXPECTF--
object(MongoDB\Driver\Manager)#%d (%d) {
  ["uri"]=>
  string(29) "mongodb://127.0.0.1/?ssl=true"
  ["cluster"]=>
  array(0) {
  }
}
object(MongoDB\Driver\Manager)#%d (%d) {
  ["uri"]=>
  string(20) "mongodb://127.0.0.1/"
  ["cluster"]=>
  array(0) {
  }
}
===DONE===
