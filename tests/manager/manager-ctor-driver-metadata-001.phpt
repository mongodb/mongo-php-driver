--TEST--
MongoDB\Driver\Manager: Pass custom handshake data
--INI--
mongodb.debug=stderr
--FILE--
<?php

$manager = new MongoDB\Driver\Manager(null, [], ['driver' => ['name' => 'test', 'version' => '0.1', 'platform' => 'mine']]);
$manager = new MongoDB\Driver\Manager(null, [], ['driver' => ['name' => 'test']]);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%A[%s]     PHONGO: DEBUG   > Setting driver handshake data: name ext-mongodb:PHP / test, version %s / 0.1, platform PHP %s / mine
%A[%s]     PHONGO: DEBUG   > Setting driver handshake data: name ext-mongodb:PHP / test, version %s, platform PHP %s
%A===DONE===%A
