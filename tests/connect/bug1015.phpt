--TEST--
PHPC-1015: Initial DNS Seedlist test
--SKIPIF--
<?php echo "skip DNS seedlist test must be run manually\n"; ?>
--FILE--
<?php
/**
 * This test requires additional configuration, and hence is not enabled by
 * default. In order for this test to succeed, you need the following line in
 * /etc/hosts:
 *
 * 192.168.112.10  localhost.test.build.10gen.cc
 *
 * The IP address needs to match the IP address that your vagrant environment
 * has created. The IP address is shown when you run "make start-servers".
 */
require_once __DIR__ . "/../utils/basic.inc";

$m = new MongoDB\Driver\Manager("mongodb+srv://test1.test.build.10gen.cc/");
$s = $m->selectServer( new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_NEAREST ) );
$servers = $m->getServers();

foreach ( $servers as $server )
{
    echo $server->getHost(), ':', $server->getPort(), "\n";
}
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%d.%d.%d.%d:27017
%d.%d.%d.%d:27018
%d.%d.%d.%d:27019
===DONE===
