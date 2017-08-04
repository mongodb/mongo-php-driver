--TEST--
MongoDB\BSON\TimestampInterface is implemented by MongoDB\BSON\Timestamp
--FILE--
<?php

$timestamp = new MongoDB\BSON\Timestamp(1234, 5678);
var_dump($timestamp instanceof MongoDB\BSON\TimestampInterface);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===
