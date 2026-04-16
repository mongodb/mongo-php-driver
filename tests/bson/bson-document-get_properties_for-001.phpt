--TEST--
MongoDB\BSON\Document get_properties_for handler (var_dump, print_r, array cast, var_export)
--FILE--
<?php

$document = MongoDB\BSON\Document::fromJSON('{"x": 1, "y": "foo"}');

var_dump($document);

print_r($document);

var_dump((array) $document);

var_export($document);
echo "\n";

$restored = eval('return ' . var_export($document, true) . ';');
var_dump($document == $restored);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Document)#%d (2) {
  ["data"]=>
  string(32) "FwAAABB4AAEAAAACeQAEAAAAZm9vAAA="
  ["value"]=>
  object(stdClass)#%d (2) {
    ["x"]=>
    int(1)
    ["y"]=>
    string(3) "foo"
  }
}
MongoDB\BSON\Document Object
(
    [data] => FwAAABB4AAEAAAACeQAEAAAAZm9vAAA=
    [value] => stdClass Object
        (
            [x] => 1
            [y] => foo
        )

)
array(1) {
  ["data"]=>
  string(32) "FwAAABB4AAEAAAACeQAEAAAAZm9vAAA="
}
%r\\?%rMongoDB\BSON\Document::__set_state(array(
   'data' => 'FwAAABB4AAEAAAACeQAEAAAAZm9vAAA=',
))
bool(true)
===DONE===
