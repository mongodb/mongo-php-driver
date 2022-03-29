--TEST--
MongoDB\Driver\BulkWrite::__construct(): let option
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    new MongoDB\Driver\BulkWrite(['let' => 'invalid']);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "let" option to be array or object, string given
===DONE===
