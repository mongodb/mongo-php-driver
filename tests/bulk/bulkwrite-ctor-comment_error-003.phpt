--TEST--
MongoDB\Driver\BulkWrite::__construct(): comment option encoding error (invalid UTF-8 scalar string)
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

echo throws(function() {
    new MongoDB\Driver\BulkWrite(['comment' => "\xc3\x28"]);
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected invalid UTF-8 in string value
===DONE===
