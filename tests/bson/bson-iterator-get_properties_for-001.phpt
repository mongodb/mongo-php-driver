--TEST--
MongoDB\BSON\Iterator get_properties_for handler (var_dump, print_r, array cast)
--FILE--
<?php

$iterator = MongoDB\BSON\Document::fromJSON('{"x": 1}')->getIterator();

var_dump($iterator);

print_r($iterator);

var_dump((array) $iterator);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Iterator)#%d (1) {
  ["bson"]=>
  object(MongoDB\BSON\Document)#%d (%d) {
    ["data"]=>
    string(%d) "%a"
    ["value"]=>
    object(stdClass)#%d (%d) {
      ["x"]=>
      int(1)
    }
  }
}
MongoDB\BSON\Iterator Object
(
    [bson] => MongoDB\BSON\Document Object
        (
            [data] => %a
            [value] => stdClass Object
                (
                    [x] => 1
                )

        )

)
array(1) {
  ["bson"]=>
  object(MongoDB\BSON\Document)#%d (%d) {
    ["data"]=>
    string(%d) "%a"
    ["value"]=>
    object(stdClass)#%d (%d) {
      ["x"]=>
      int(1)
    }
  }
}
===DONE===
