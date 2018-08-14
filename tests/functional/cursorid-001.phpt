--TEST--
Sorting single field, ascending, using the Cursor Iterator
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
loadFixtures($manager);

$query = new MongoDB\Driver\Query(array(), array(
    'projection' => array('_id' => 0, 'username' => 1),
    'sort' => array('username' => 1),
    'batchSize' => 11,
    'limit' => 110,
));

$cursor = $manager->executeQuery(NS, $query);

$cursorid = $cursor->getId();
$s1 = (string)$cursorid;
var_dump(
    $cursorid,
    $s1
);
var_dump($s1 > 0); 

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\CursorId)#%d (%d) {
  ["id"]=>
  %rint\(\d+\)|string\(\d+\) "\d+"%r
}
string(%d) "%d"
bool(true)
===DONE===
