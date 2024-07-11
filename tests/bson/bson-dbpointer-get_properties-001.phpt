--TEST--
MongoDB\BSON\DBPointer get_properties handler (get_object_vars)
--FILE--
<?php

$document = MongoDB\BSON\Document::fromJSON('{"dbptr":{"$dbPointer":{"$ref":"phongo.test","$id":{"$oid":"5a2e78accd485d55b405ac12"}}}}')->toPHP();
$dbptr = $document->dbptr;

var_dump(get_object_vars($dbptr));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(2) {
  ["ref"]=>
  string(11) "phongo.test"
  ["id"]=>
  string(24) "5a2e78accd485d55b405ac12"
}
===DONE===
