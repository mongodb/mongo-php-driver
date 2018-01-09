--TEST--
MongoDB\BSON\DBPointer get_properties handler (foreach)
--FILE--
<?php

$document = MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{"dbptr":{"$dbPointer":{"$ref":"phongo.test","$id":{"$oid":"5a2e78accd485d55b405ac12"}}}}'));
$dbptr = $document->dbptr;

foreach ($dbptr as $key => $value) {
    var_dump($key);
    var_dump($value);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(3) "ref"
string(11) "phongo.test"
string(2) "id"
string(24) "5a2e78accd485d55b405ac12"
===DONE===
