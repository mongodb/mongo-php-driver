--TEST--
MongoDB\Driver\Session::startTransaction() does not support sharded clusters
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto() ?>
<?php skip_if_not_mongos(); ?>
<?php skip_if_server_version('<', '4.0'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
$session = $manager->startSession();

echo throws(function() use ($session) {
    $session->startTransaction();
}, 'MongoDB\Driver\Exception\RuntimeException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\RuntimeException
PHP MongoDB driver %s does not support running multi-document transactions on sharded clusters
===DONE===
