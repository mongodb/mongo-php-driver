--TEST--
MongoDB\Driver\Cursor query result iteration through NoRewindIterator
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(array('_id' => 1, 'x' => 1));
$bulk->insert(array('_id' => 2, 'x' => 1));
$manager->executeBulkWrite(NS, $bulk);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array("x" => 1)));

/* IteratorIterator requires either rewind() or next() to be called at least
 * once to populate its current.data pointer, which valid() checks. Since next()
 * would skip the first element and NoRewindIterator::rewind() is a NOP, we must
 * explicitly call IteratorIterator::rewind() before composing it. */
$iteratorIterator = new IteratorIterator($cursor);
$iteratorIterator->rewind();

$noRewindIterator = new NoRewindIterator($iteratorIterator);

foreach ($noRewindIterator as $document) {
    var_dump($document);
}

/* NoRewindIterator::rewind() is a NOP, so attempting to iterate a second time
 * or calling rewind() directly accomplishes nothing. That said, it does avoid
 * the exception one would otherwise get invoking the rewind handler after
 * iteration has started. */
foreach ($noRewindIterator as $document) {
    var_dump($document);
}

$noRewindIterator->rewind();

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (2) {
  ["_id"]=>
  int(1)
  ["x"]=>
  int(1)
}
object(stdClass)#%d (2) {
  ["_id"]=>
  int(2)
  ["x"]=>
  int(1)
}
===DONE===
