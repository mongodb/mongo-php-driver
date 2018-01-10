--TEST--
PHPC-1067: BSON document produces driver segfault with insert
--FILE--
<?php
$x = [
    '_______'                             => new MongoDB\BSON\ObjectID('111111111111111111111111'),
    '___________________________________' => new MongoDB\BSON\Regex('_______________________________________________________', 'i'),
];
 
$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert($x);
?>
==DONE==
<?php exit(0); ?>
--EXPECT--
==DONE==
