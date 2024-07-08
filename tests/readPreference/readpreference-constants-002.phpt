--TEST--
MongoDB\Driver\ReadPreference deprecated constants
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.3.0'); ?>
--FILE--
<?php

var_dump(MongoDB\Driver\ReadPreference::RP_PRIMARY);
var_dump(MongoDB\Driver\ReadPreference::RP_PRIMARY_PREFERRED);
var_dump(MongoDB\Driver\ReadPreference::RP_SECONDARY);
var_dump(MongoDB\Driver\ReadPreference::RP_SECONDARY_PREFERRED);
var_dump(MongoDB\Driver\ReadPreference::RP_NEAREST);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: Constant MongoDB\Driver\ReadPreference::RP_PRIMARY is deprecated in %s
int(1)

Deprecated: Constant MongoDB\Driver\ReadPreference::RP_PRIMARY_PREFERRED is deprecated in %s
int(5)

Deprecated: Constant MongoDB\Driver\ReadPreference::RP_SECONDARY is deprecated in %s
int(2)

Deprecated: Constant MongoDB\Driver\ReadPreference::RP_SECONDARY_PREFERRED is deprecated in %s
int(6)

Deprecated: Constant MongoDB\Driver\ReadPreference::RP_NEAREST is deprecated in %s
int(10)
===DONE===
