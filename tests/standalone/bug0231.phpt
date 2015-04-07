--TEST--
Multiple managers sharing streams: Using stream after closing manager
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE)?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager  = new MongoDB\Driver\Manager(STANDALONE);
$manager2 = new MongoDB\Driver\Manager(STANDALONE);


$listdatabases = new MongoDB\Driver\Command(array("listDatabases" => 1));
$retval        = $manager->executeCommand("admin", $listdatabases);
$retval        = $manager2->executeCommand("admin", $listdatabases);

foreach($retval as $database) {
}

$manager = null;
$retval  = $manager2->executeCommand("admin", $listdatabases);

foreach($retval as $database) {
}
echo "All Good!\n";
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
All Good!
===DONE===
