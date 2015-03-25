--TEST--
Sorting single field, ascending, using the Cursor Iterator
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
<?php require "tests/utils/fixtures-users.inc" ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$query = new MongoDB\Driver\Query(array(), array(
    'projection' => array('_id' => 0, 'username' => 1),
    'sort' => array('username' => 1),
    'batchSize' => 11,
    'limit' => 110,
));

$qr = $manager->executeQuery(NS, $query);
$cursor = $qr->getIterator();
$cursorid = $cursor->getId();
$s1 = (string)$cursorid;
var_dump(
    $cursorid,
    $s1
);
$cursor->kill();


?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\CursorId)#%d (%d) {
  ["id"]=>
  %s(%d)
}
string(%d) "%d"
===DONE===
