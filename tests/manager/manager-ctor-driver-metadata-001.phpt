--TEST--
MongoDB\Driver\Manager: Pass custom handshake data
--DESCRIPTION--
This test matches spaces at the end of the handshake data it appends. The only
way to see final output is by checking against the binary socket communication:
[2021-02-17T13:57:57.155166+00:00]     socket: TRACE   > 00100:  66 6f 72 6d 00 76 00 00  00 50 48 50 20 37 2e 34  f o r m . v . .  . P H P   7 . 4
[2021-02-17T13:57:57.155182+00:00]     socket: TRACE   > 00110:  2e 31 35 20 2f 20 6d 69  6e 65 20 63 66 67 3d 30  . 1 5   /   m i  n e   c f g = 0

Since matching this is not trivial, we're happy matching the trailing space at
the end of each handshake data item.
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
%A[%s]     PHONGO: DEBUG   > Setting driver handshake data: { name: 'ext-mongodb:PHP / test ', version: '%s / 0.1 ', platform: 'PHP %s / mine ' }
%A[%s]     PHONGO: DEBUG   > Setting driver handshake data: { name: 'ext-mongodb:PHP / test ', version: '%s ', platform: 'PHP %s ' }
%A===DONE===%A
