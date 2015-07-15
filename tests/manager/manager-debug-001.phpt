--TEST--
MongoDB\Driver\Manager: Writing debug log files
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$name = tempnam(sys_get_temp_dir(), "PHONGO");
unlink($name);
mkdir($name);
$manager = new MongoDB\Driver\Manager(STANDALONE, array(), array("debug" => $name));
$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(array('_id' => 1, 'x' => 1));
$result = $manager->executeBulkWrite(NS, $bulk);
foreach(glob($name."/*") as $file);
$content = file($file);
unlink($file);
rmdir($name);

echo $content[0];
foreach($content as $line) {
    if (strpos($line, "mongoc_bulk_operation_execute")) {
        echo $line;
    }
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
[%s]     PHONGO: DEBUG   > Creating Manager, phongo-0.%d.%d[%s] - mongoc-1.%s, libbson-1.%s
[%s]     mongoc: TRACE   > ENTRY: mongoc_bulk_operation_execute():%d
[%s]     mongoc: TRACE   >  EXIT: mongoc_bulk_operation_execute():%d
===DONE===
