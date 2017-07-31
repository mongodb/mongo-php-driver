--TEST--
MongoDB\BSON\JavascriptInterface example with MongoDB\BSON\TypeWrapper
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class MyJavascript implements MongoDB\BSON\TypeWrapper, MongoDB\BSON\JavascriptInterface
{
    private $javascript;

    public function __construct($code, $scope)
    {
        $this->javascript = new MongoDB\BSON\Javascript($code, $scope);
    }

    public static function createFromBSONType(MongoDB\BSON\Type $type)
    {
        if ( ! $type instanceof MongoDB\BSON\Javascript) {
            throw new InvalidArgumentException('Cannot create MyJavascript from ' . get_class($type));
        }

        return new self($type->getCode(), $type->getScope());
    }

    public function toBSONType()
    {
        return $this->javascript;
    }

    public function getCode()
    {
        return $this->javascript->getCode();
    }

    public function getScope()
    {
        return $this->javascript->getScope();
    }

    public function __toString()
    {
        return (string) $this->javascript;
    }
}

$javascript = new MongoDB\BSON\Javascript('function foo() { return bar; }', ['bar' => 1]);
$myJavascript = new MyJavascript('function foo() { return bar; }', ['bar' => 1]);

echo "\nTesting MyJavascript and Javascript are equivalent\n";
var_dump($javascript->getCode() === $myJavascript->getCode());
// Do not use "===" since each scope is a different stdClass instance
var_dump($javascript->getScope() == $myJavascript->getScope());
var_dump((string) $javascript === (string) $myJavascript);

echo "\nTesting MyJavascript and Javascript produce the same BSON\n";
var_dump(fromPHP(['x' => $javascript]) === fromPHP(['x' => $myJavascript]));

echo "\nTesting Javascript to BSON to MyJavascript\n";
var_dump(toPHP(fromPHP(['x' => $javascript]), ['types' => ['Javascript' => 'MyJavascript']])->x);

echo "\nTesting MyJavascript to BSON to MyJavascript\n";
var_dump(toPHP(fromPHP(['x' => $myJavascript]), ['types' => ['Javascript' => 'MyJavascript']])->x);

echo "\nTesting MyJavascript to BSON to Javascript\n";
var_dump(toPHP(fromPHP(['x' => $myJavascript]))->x);

echo "\nTesting MyJavascript::createFromBSONType() expects Javascript\n";
echo throws(function() {
    toPHP(fromPHP(['x' => new MongoDB\BSON\MinKey]), ['types' => ['MinKey' => 'MyJavascript']]);
}, 'InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing MyJavascript and Javascript are equivalent
bool(true)
bool(true)
bool(true)

Testing MyJavascript and Javascript produce the same BSON
bool(true)

Testing Javascript to BSON to MyJavascript
object(MyJavascript)#%d (%d) {
  ["javascript":"MyJavascript":private]=>
  object(MongoDB\BSON\Javascript)#%d (%d) {
    ["code"]=>
    string(30) "function foo() { return bar; }"
    ["scope"]=>
    object(stdClass)#%d (%d) {
      ["bar"]=>
      int(1)
    }
  }
}

Testing MyJavascript to BSON to MyJavascript
object(MyJavascript)#%d (%d) {
  ["javascript":"MyJavascript":private]=>
  object(MongoDB\BSON\Javascript)#%d (%d) {
    ["code"]=>
    string(30) "function foo() { return bar; }"
    ["scope"]=>
    object(stdClass)#%d (%d) {
      ["bar"]=>
      int(1)
    }
  }
}

Testing MyJavascript to BSON to Javascript
object(MongoDB\BSON\Javascript)#%d (%d) {
  ["code"]=>
  string(30) "function foo() { return bar; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
    ["bar"]=>
    int(1)
  }
}

Testing MyJavascript::createFromBSONType() expects Javascript
OK: Got InvalidArgumentException
Cannot create MyJavascript from MongoDB\BSON\MinKey
===DONE===
