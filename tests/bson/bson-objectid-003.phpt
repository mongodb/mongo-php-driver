--TEST--
BSON BSON\ObjectID #001 construction with string argument
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$classname = BSON_NAMESPACE . "\\ObjectID";

class StringObject
{
    private $value;

    public function __construct($value)
    {
        $this->value = (string) $value;
    }

    public function __toString()
    {
        return $this->value;
    }
}

$oid = new $classname('53e2a1c40640fd72175d4603');
$str = new StringObject('53e2a1c40640fd72175d4603');

var_dump($oid);
var_dump(new $classname($oid));
var_dump(new $classname($str));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\ObjectID)#%d (%d) {
  ["oid"]=>
  string(24) "53e2a1c40640fd72175d4603"
}
object(MongoDB\BSON\ObjectID)#%d (%d) {
  ["oid"]=>
  string(24) "53e2a1c40640fd72175d4603"
}
object(MongoDB\BSON\ObjectID)#%d (%d) {
  ["oid"]=>
  string(24) "53e2a1c40640fd72175d4603"
}
===DONE===
