--TEST--
MongoDB\BSON\ObjectID #003 construction with string argument
--FILE--
<?php

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

$oid = new MongoDB\BSON\ObjectID('53e2a1c40640fd72175d4603');
$str = new StringObject('53e2a1c40640fd72175d4603');

var_dump($oid);
var_dump(new MongoDB\BSON\ObjectID($oid));
var_dump(new MongoDB\BSON\ObjectID($str));

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
