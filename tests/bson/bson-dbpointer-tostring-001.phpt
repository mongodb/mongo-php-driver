--TEST--
MongoDB\BSON\DBPointer::__toString()
--FILE--
<?php

$dbref = MongoDB\BSON\Document::fromJSON('{ "dbref": {"$dbPointer": {"$ref": "phongo.test", "$id" : { "$oid" : "5a2e78accd485d55b4050000" }  }} }')->toPHP()->dbref;
var_dump((string) $dbref);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(38) "[phongo.test/5a2e78accd485d55b4050000]"
===DONE===
