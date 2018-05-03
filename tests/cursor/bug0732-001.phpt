--TEST--
PHPC-732: Possible mongoc_client_t use-after-free with Cursor wrapped in generator
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

function wrapCursor(MongoDB\Driver\Cursor $cursor)
{
    foreach ($cursor as $key => $value) {
        yield $key => $value;
    }
}

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 2]);
$bulk->insert(['_id' => 3]);
$manager->executeBulkWrite(NS, $bulk);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([], ['batchSize' => 2]));
$generator = wrapCursor($cursor);

foreach ($generator as $value) {
    echo "Exiting during first iteration on generator\n";
    exit(0);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Exiting during first iteration on generator
