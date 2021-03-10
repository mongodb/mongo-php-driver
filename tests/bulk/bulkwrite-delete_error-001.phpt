--TEST--
MongoDB\Driver\BulkWrite::delete() with invalid options
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$bulk = new MongoDB\Driver\BulkWrite;

echo throws(function() use ($bulk) {
    $bulk->delete(['x' => 1], ['collation' => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->delete(['x' => 1], ['hint' => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "collation" option to be array or object, int%S given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "hint" option to be string, array, or object, int%S given
===DONE===
