--TEST--
MongoDB\Driver\Manager::__construct(): ssl option does not require driverOptions
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_ssl(); ?>
--FILE--
<?php

/* The purpose of this test is to demonstrate that the SSL option does not
 * require driver options, so we simply dump the constructed Manager without
 * actually connecting. */
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
