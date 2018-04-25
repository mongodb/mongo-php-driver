--TEST--
MongoDB\Driver\WriteResult::isAcknowledged()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$wcs = [
    new MongoDB\Driver\WriteConcern(0),
    new MongoDB\Driver\WriteConcern(1),
];

foreach ($wcs as $wc) {
    $bulk = new MongoDB\Driver\BulkWrite;
    $bulk->insert(['x' => 1]);

    $result = $manager->executeBulkWrite(NS, $bulk, $wc);

    var_dump($result->isAcknowledged());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(false)
bool(true)
===DONE===
