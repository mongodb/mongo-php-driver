--TEST--
Test for PHPC-347: Memory leak decoding empty buffer
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

try {
    var_dump(toPHP(''));
} catch (MongoDB\Driver\Exception\UnexpectedValueException $e) {
    echo $e->getMessage(), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Could not read document from BSON reader
===DONE===
