--TEST--
MongoDB\Driver\Manager: maxStalenessMS
--FILE--
<?php
// They both should work, and produce no output. We're not testing whether the flag actually does something
$manager = new MongoDB\Driver\Manager("mongodb://localhost/?readPreference=SECONDARY&maxStalenessMS=1231");
$manager = new MongoDB\Driver\Manager("mongodb://localhost/?readPreference=SECONDARY", [ 'maxStalenessMS' => 1231 ] );
$manager = new MongoDB\Driver\Manager("mongodb://localhost/?readPreference=SECONDARY&maxstalenessms=1231");
$manager = new MongoDB\Driver\Manager("mongodb://localhost/?readPreference=SECONDARY", [ 'maxstalenessms' => 1231 ] );
?>
==DONE==
--EXPECTF--
==DONE==
