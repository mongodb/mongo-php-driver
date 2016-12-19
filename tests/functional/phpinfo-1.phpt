--TEST--
phpinfo()
--SKIPIF--
<?php if (defined("HHVM_VERSION_ID")) exit("skip HHVM does not do phpinfo() this way"); ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

phpinfo();

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%a
mongodb

MongoDB support => enabled
MongoDB extension version => 1.%d.%d%S
MongoDB extension stability => %s
libmongoc bundled version => 1.%s
libbson bundled version => 1.%s

Directive => Local Value => Master Value
mongodb.debug => no value => no value

%a
===DONE===
