--TEST--
PHPC-2746: Non-string TLS driver options are freed as interior pointers
--DESCRIPTION--
See tests/bson/bug2746-001.phpt: phongo_fetch_string() takes ownership of the
converted string returned by php_array_zval_to_string(), which used to be an
interior pointer into a zend_string allocation.
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_ssl(['OpenSSL']); /* ca_dir is unsupported by Secure Channel and Secure Transport */ ?>
--ENV--
USE_ZEND_ALLOC=0
--FILE--
<?php

var_dump(new MongoDB\Driver\Manager(null, [], ['ca_dir' => 1234567890]));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\Manager)#%d (%d) {
  ["uri"]=>
  string(20) "mongodb://127.0.0.1/"
  ["cluster"]=>
  array(0) {
  }
}
===DONE===
