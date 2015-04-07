--TEST--
MongoDB\Driver\Cursor destruct should kill a live cursor
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

function getNumOpenCursors(MongoDB\Driver\Manager $manager)
{
    $cursor = $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(array('serverStatus' => 1)));
    $result = current($cursor->toArray());
    return $result['cursors']->totalOpen;
}

$manager = new MongoDB\Driver\Manager(STANDALONE);

$manager->executeInsert(NS, array('_id' => 1));
$manager->executeInsert(NS, array('_id' => 2));
$manager->executeInsert(NS, array('_id' => 3));

$numOpenCursorsBeforeQuery = getNumOpenCursors($manager);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array(), array('batchSize' => 2)));

var_dump($cursor->isDead());
var_dump(getNumOpenCursors($manager) == $numOpenCursorsBeforeQuery + 1);

unset($cursor);

var_dump(getNumOpenCursors($manager) == $numOpenCursorsBeforeQuery);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(false)
bool(true)
bool(true)
===DONE===
