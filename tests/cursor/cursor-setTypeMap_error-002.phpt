--TEST--
Cursor::setTypeMap() error does not alter current element
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['_id' => 1]);
$manager->executeBulkWrite(NS, $bulk);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));
$iterator = new IteratorIterator($cursor);

$iterator->rewind();
var_dump($iterator->current());

echo throws(function() use ($cursor) {
    $cursor->setTypeMap(['root' => 'MissingClass']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

/* IteratorIterator only invokes spl_dual_it_fetch() for rewind() and next().
 * We rewind a second time to ensure that the Cursor iterator's current element
 * is fetched again is remains unchanged. */
$iterator->rewind();
var_dump($iterator->current());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["_id"]=>
  int(1)
}
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MissingClass does not exist
object(stdClass)#%d (%d) {
  ["_id"]=>
  int(1)
}
===DONE===
