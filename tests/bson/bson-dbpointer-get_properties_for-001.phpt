--TEST--
MongoDB\BSON\DBPointer get_properties_for handler (var_dump, print_r, array cast, var_export)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$dbpointer = createDBPointer('phongo.test', '5a2e78accd485d55b4050000');

var_dump($dbpointer);

print_r($dbpointer);

var_dump((array) $dbpointer);

var_export($dbpointer);
echo "\n";

$restored = eval('return ' . var_export($dbpointer, true) . ';');
var_dump($dbpointer == $restored);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\DBPointer)#%d (2) {
  ["ref"]=>
  string(11) "phongo.test"
  ["id"]=>
  string(24) "5a2e78accd485d55b4050000"
}
MongoDB\BSON\DBPointer Object
(
    [ref] => phongo.test
    [id] => 5a2e78accd485d55b4050000
)
array(2) {
  ["ref"]=>
  string(11) "phongo.test"
  ["id"]=>
  string(24) "5a2e78accd485d55b4050000"
}
%r\\?%rMongoDB\BSON\DBPointer::__set_state(array(
   'ref' => 'phongo.test',
   'id' => '5a2e78accd485d55b4050000',
))
bool(true)
===DONE===
