--TEST--
MongoDB\BSON\RegexInterface example with MongoDB\BSON\TypeWrapper
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class MyRegex implements MongoDB\BSON\TypeWrapper, MongoDB\BSON\RegexInterface
{
    private $regex;

    public function __construct($pattern, $flags)
    {
        $this->regex = new MongoDB\BSON\Regex($pattern, $flags);
    }

    public static function createFromBSONType(MongoDB\BSON\Type $type)
    {
        if ( ! $type instanceof MongoDB\BSON\Regex) {
            throw new InvalidArgumentException('Cannot create MyRegex from ' . get_class($type));
        }

        return new self($type->getPattern(), $type->getFlags());
    }

    public function toBSONType()
    {
        return $this->regex;
    }

    public function getPattern()
    {
        return $this->regex->getPattern();
    }

    public function getFlags()
    {
        return $this->regex->getFlags();
    }

    public function __toString()
    {
        return (string) $this->regex;
    }
}

$regex = new MongoDB\BSON\Regex('pattern', 'i');
$myRegex = new MyRegex('pattern', 'i');

echo "\nTesting MyRegex and Regex are equivalent\n";
var_dump($regex->getPattern() === $myRegex->getPattern());
var_dump($regex->getFlags() === $myRegex->getFlags());
var_dump((string) $regex === (string) $myRegex);

echo "\nTesting MyRegex and Regex produce the same BSON\n";
var_dump(fromPHP(['x' => $regex]) === fromPHP(['x' => $myRegex]));

echo "\nTesting Regex to BSON to MyRegex\n";
var_dump(toPHP(fromPHP(['x' => $regex]), ['types' => ['Regex' => 'MyRegex']])->x);

echo "\nTesting MyRegex to BSON to MyRegex\n";
var_dump(toPHP(fromPHP(['x' => $myRegex]), ['types' => ['Regex' => 'MyRegex']])->x);

echo "\nTesting MyRegex to BSON to Regex\n";
var_dump(toPHP(fromPHP(['x' => $myRegex]))->x);

echo "\nTesting MyRegex::createFromBSONType() expects Regex\n";
echo throws(function() {
    toPHP(fromPHP(['x' => new MongoDB\BSON\MinKey]), ['types' => ['MinKey' => 'MyRegex']]);
}, 'InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing MyRegex and Regex are equivalent
bool(true)
bool(true)
bool(true)

Testing MyRegex and Regex produce the same BSON
bool(true)

Testing Regex to BSON to MyRegex
object(MyRegex)#%d (%d) {
  ["regex":"MyRegex":private]=>
  object(MongoDB\BSON\Regex)#%d (%d) {
    ["pattern"]=>
    string(7) "pattern"
    ["flags"]=>
    string(1) "i"
  }
}

Testing MyRegex to BSON to MyRegex
object(MyRegex)#%d (%d) {
  ["regex":"MyRegex":private]=>
  object(MongoDB\BSON\Regex)#%d (%d) {
    ["pattern"]=>
    string(7) "pattern"
    ["flags"]=>
    string(1) "i"
  }
}

Testing MyRegex to BSON to Regex
object(MongoDB\BSON\Regex)#%d (%d) {
  ["pattern"]=>
  string(7) "pattern"
  ["flags"]=>
  string(1) "i"
}

Testing MyRegex::createFromBSONType() expects Regex
OK: Got InvalidArgumentException
Cannot create MyRegex from MongoDB\BSON\MinKey
===DONE===
