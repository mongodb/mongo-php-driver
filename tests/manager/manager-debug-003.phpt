--TEST--
MongoDB\Driver\Manager: mongodb.debug=stderr (date format)
--INI--
mongodb.debug=stderr
--FILE--
<?php
$manager = new MongoDB\Driver\Manager;
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
[%r(\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}.\d{6}\+00:00)%r]%A
===DONE===%A
