--TEST--
MongoDB\Driver\Cursor command result iteration with batchSize requiring getmore with non-full batches
--SKIPIF--
<?php exit("skip Temporarily disabled until related libmongoc issues are resolved"); ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php START("THROWAWAY"); CLEANUP(THROWAWAY); LOAD(THROWAWAY); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(THROWAWAY);

$query = new MongoDB\Driver\Query(array(), array(
    'projection' => array('_id' => 0, 'username' => 1),
    'sort' => array('username' => 1),
));

$cursor = $manager->executeQuery(NS, $query);

failGetMore($manager);
throws(function() use ($cursor) {
    foreach ($cursor as $document) {
        echo $document['username'] . "\n";
    }
}, "MongoDB\Driver\Exception\ConnectionException");


?>
===DONE===
<?php DELETE("THROWAWAY"); ?>
<?php exit(0); ?>
--CLEANUP--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php DELETE("THROWAWAY"); ?>
--EXPECT--
