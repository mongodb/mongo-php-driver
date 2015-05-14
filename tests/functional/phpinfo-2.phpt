--TEST--
phpinfo()
--INI--
mongodb.debug=stderr
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

ini_set("mongodb.debug", "stdout");
phpinfo();

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%a
mongodb

mongodb support => enabled
mongodb version => 0.%d.%d
mongodb stability => %s
libmongoc version => 1.%s
libbson version => 1.%s

Directive => Local Value => Master Value
mongodb.debug => stdout => stderr

%a
===DONE===
