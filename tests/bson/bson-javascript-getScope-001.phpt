--TEST--
MongoDB\BSON\Javascript::getScope()
--FILE--
<?php

$tests = [
    ['function(bar) { return bar; }', null],
    ['function(bar) { return bar; }', []],
    ['function() { return foo; }', ['foo' => 42]],
    ['function() { return id; }', ['id' => new MongoDB\BSON\ObjectId('53e2a1c40640fd72175d4603')]],
];

foreach ($tests as $test) {
    list($code, $scope) = $test;

    $js = new MongoDB\BSON\Javascript($code, $scope);
    var_dump($js->getScope());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
NULL
object(stdClass)#%d (%d) {
}
object(stdClass)#%d (%d) {
  ["foo"]=>
  int(42)
}
object(stdClass)#%d (%d) {
  ["id"]=>
  object(MongoDB\BSON\ObjectId)#%d (%d) {
    ["oid"]=>
    string(24) "53e2a1c40640fd72175d4603"
  }
}
===DONE===
