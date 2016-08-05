--TEST--
PHPC-528: Cannot append reference to BSON
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$embedded = ['foo'];
$data = ['embedded' => &$embedded];

$bson = fromPHP($data);
echo toJson(fromPHP($data)), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
{ "embedded" : [ "foo" ] }
===DONE===
