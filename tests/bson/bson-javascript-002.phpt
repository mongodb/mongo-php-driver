--TEST--
MongoDB\BSON\Javascript debug handler
--FILE--
<?php

$tests = array(
    array(
        'function(bar) { return bar; }',
        array(),
    ),
    array(
        'function() { return foo; }',
        array('foo' => 42),
    ),
    array(
        'function() { return id; }',
        array('id' => new MongoDB\BSON\ObjectId('53e2a1c40640fd72175d4603')),
    ),
);

foreach ($tests as $test) {
    list($code, $scope) = $test;

    $js = new MongoDB\BSON\Javascript($code, $scope);
    var_dump($js);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Javascript)#%d (%d) {
  ["code"]=>
  string(29) "function(bar) { return bar; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
  }
}
object(MongoDB\BSON\Javascript)#%d (%d) {
  ["code"]=>
  string(26) "function() { return foo; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
    ["foo"]=>
    int(42)
  }
}
object(MongoDB\BSON\Javascript)#%d (%d) {
  ["code"]=>
  string(25) "function() { return id; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
    ["id"]=>
    object(MongoDB\BSON\ObjectId)#%d (%d) {
      ["oid"]=>
      string(24) "53e2a1c40640fd72175d4603"
    }
  }
}
===DONE===
