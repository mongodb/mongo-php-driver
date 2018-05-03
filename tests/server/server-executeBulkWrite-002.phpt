--TEST--
MongoDB\Driver\Server::executeBulkWrite() with write concern (standalone)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
$primary = $manager->selectServer(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY));

$writeConcerns = array(0, 1);

foreach ($writeConcerns as $writeConcern) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(array('wc' => $writeConcern));

    $result = $primary->executeBulkWrite(NS, $bulk, new MongoDB\Driver\WriteConcern($writeConcern));
    var_dump($result->isAcknowledged());
    var_dump($result->getInsertedCount());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(false)
NULL
bool(true)
int(1)
===DONE===
