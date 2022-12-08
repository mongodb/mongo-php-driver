--TEST--
MongoDB\BSON\Document::toPHP(): Tests from serialization specification
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

class MyClass
{
}

#[\AllowDynamicProperties]
class YourClass implements MongoDB\BSON\Unserializable
{
    function bsonUnserialize(array $data): void
    {
        foreach ($data as $key => $value) {
            $this->$key = $value;
        }
        $this->unserialized = true;
    }
}

#[\AllowDynamicProperties]
class OurClass implements MongoDB\BSON\Persistable
{
    function bsonSerialize(): array
    {
        // Not tested with this test, so return empty array
        return [];
    }

    function bsonUnserialize(array $data): void
    {
        foreach ($data as $key => $value) {
            $this->$key = $value;
        }
        $this->unserialized = true;
    }
}

class TheirClass extends OurClass
{
}

// Create base64-encoded class names for __pclass field's binary data
$bMyClass = base64_encode('MyClass');
$bYourClass = base64_encode('YourClass');
$bOurClass = base64_encode('OurClass');
$bTheirClass = base64_encode('TheirClass');
$bInterface = base64_encode('MongoDB\BSON\Unserializable');

$testGroups = [
    [
        'name' => 'DEFAULT TYPEMAP',
        'typemap' => [],
        'tests' => [
            '{ "foo": "yes", "bar" : false }',
            '{ "foo": "no", "array" : [ 5, 6 ] }',
            '{ "foo": "no", "obj" : { "embedded" : 4.125 } }',
            '{ "foo": "yes", "__pclass": "MyClass" }',
            '{ "foo": "yes", "__pclass": { "$binary": "' . $bMyClass . '", "$type": "80" } }',
            '{ "foo": "yes", "__pclass": { "$binary": "' . $bYourClass . '", "$type": "80" } }',
            '{ "foo": "yes", "__pclass": { "$binary": "' . $bOurClass . '", "$type": "80" } }',
            '{ "foo": "yes", "__pclass": { "$binary": "' . $bYourClass . '", "$type": "44" } }',
        ],
    ],
    [
        'name' => 'NONEXISTING CLASS',
        'typemap' => ['root' => 'MissingClass'],
        'tests' => [
            '{ "foo": "yes" }',
        ],
    ],
    [
        'name' => 'DOES NOT IMPLEMENT UNSERIALIZABLE',
        'typemap' => ['root' => 'MyClass'],
        'tests' => [
            '{ "foo": "yes", "__pclass": { "$binary": "' . $bMyClass . '", "$type": "80" } }',
        ],
    ],
    [
        'name' => 'IS NOT A CONCRETE CLASS',
        'typemap' => ['root' => 'MongoDB\BSON\Unserializable'],
        'tests' => [
            '{ "foo": "yes" }',
        ],
    ],
    [
        'name' => 'IS NOT A CONCRETE CLASS VIA PCLASS',
        'typemap' => ['root' => 'YourClass'],
        'tests' => [
            '{ "foo": "yes", "__pclass" : { "$binary": "' . $bInterface . '", "$type": "80" } }',
        ],
    ],
    [
        'name' => 'PCLASS OVERRIDES TYPEMAP (1)',
        'typemap' => ['root' => 'YourClass'],
        'tests' => [
            '{ "foo": "yes", "__pclass" : { "$binary": "' . $bMyClass . '", "$type": "80" } }',
            '{ "foo": "yes", "__pclass" : { "$binary": "' . $bOurClass . '", "$type": "80" } }',
            '{ "foo": "yes", "__pclass" : { "$binary": "' . $bTheirClass . '", "$type": "80" } }',
            '{ "foo": "yes", "__pclass" : { "$binary": "' . $bYourClass . '", "$type": "80" } }',
        ],
    ],
    [
        'name' => 'PCLASS OVERRIDES TYPEMAP (2)',
        'typemap' => ['root' => 'OurClass'],
        'tests' => [
            '{ "foo": "yes", "__pclass" : { "$binary": "' . $bTheirClass . '", "$type": "80" } }',
        ],
    ],
    [
        'name' => 'OBJECTS AS ARRAY',
        'typemap' => ['root' => 'array', 'document' => 'array'],
        'tests' => [
            '{ "foo": "yes", "bar" : false }',
            '{ "foo": "no", "array" : [ 5, 6 ] }',
            '{ "foo": "no", "obj" : { "embedded" : 4.125 } }',
            '{ "foo": "yes", "__pclass": "MyClass" }',
            '{ "foo": "yes", "__pclass" : { "$binary": "' . $bMyClass . '", "$type": "80" } }',
            '{ "foo": "yes", "__pclass" : { "$binary": "' . $bOurClass . '", "$type": "80" } }',
        ],
    ],
    [
        'name' => 'OBJECTS AS STDCLASS',
        'typemap' => ['root' => 'object', 'document' => 'object'],
        'tests' => [
            '{ "foo": "yes", "__pclass" : { "$binary": "' . $bMyClass . '", "$type": "80" } }',
            '{ "foo": "yes", "__pclass" : { "$binary": "' . $bOurClass . '", "$type": "80" } }',
        ],
    ],
];

foreach ($testGroups as $testGroup) {
    printf("=== %s ===\n\n", $testGroup['name']);

    foreach ($testGroup['tests'] as $test) {
        echo $test, "\n";

        $bson = MongoDB\BSON\Document::fromJSON($test);
        try {
            var_dump($bson->toPHP($testGroup['typemap']));
        } catch (MongoDB\Driver\Exception\Exception $e) {
            echo $e->getMessage(), "\n";
        }

        echo "\n";
    }

    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
=== DEFAULT TYPEMAP ===

{ "foo": "yes", "bar" : false }
object(stdClass)#%d (2) {
  ["foo"]=>
  string(3) "yes"
  ["bar"]=>
  bool(false)
}

{ "foo": "no", "array" : [ 5, 6 ] }
object(stdClass)#%d (2) {
  ["foo"]=>
  string(2) "no"
  ["array"]=>
  array(2) {
    [0]=>
    int(5)
    [1]=>
    int(6)
  }
}

{ "foo": "no", "obj" : { "embedded" : 4.125 } }
object(stdClass)#%d (2) {
  ["foo"]=>
  string(2) "no"
  ["obj"]=>
  object(stdClass)#%d (1) {
    ["embedded"]=>
    float(4.125)
  }
}

{ "foo": "yes", "__pclass": "MyClass" }
object(stdClass)#%d (2) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  string(7) "MyClass"
}

{ "foo": "yes", "__pclass": { "$binary": "TXlDbGFzcw==", "$type": "80" } }
object(stdClass)#%d (2) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(MongoDB\BSON\Binary)#%d (2) {
    ["data"]=>
    string(7) "MyClass"
    ["type"]=>
    int(128)
  }
}

{ "foo": "yes", "__pclass": { "$binary": "WW91ckNsYXNz", "$type": "80" } }
object(stdClass)#%d (2) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(MongoDB\BSON\Binary)#%d (2) {
    ["data"]=>
    string(9) "YourClass"
    ["type"]=>
    int(128)
  }
}

{ "foo": "yes", "__pclass": { "$binary": "T3VyQ2xhc3M=", "$type": "80" } }
object(OurClass)#%d (3) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(MongoDB\BSON\Binary)#%d (2) {
    ["data"]=>
    string(8) "OurClass"
    ["type"]=>
    int(128)
  }
  ["unserialized"]=>
  bool(true)
}

{ "foo": "yes", "__pclass": { "$binary": "WW91ckNsYXNz", "$type": "44" } }
object(stdClass)#%d (2) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(MongoDB\BSON\Binary)#%d (2) {
    ["data"]=>
    string(9) "YourClass"
    ["type"]=>
    int(68)
  }
}


=== NONEXISTING CLASS ===

{ "foo": "yes" }
Class MissingClass does not exist


=== DOES NOT IMPLEMENT UNSERIALIZABLE ===

{ "foo": "yes", "__pclass": { "$binary": "TXlDbGFzcw==", "$type": "80" } }
Class MyClass does not implement MongoDB\BSON\Unserializable


=== IS NOT A CONCRETE CLASS ===

{ "foo": "yes" }
Interface MongoDB\BSON\Unserializable is not instantiatable


=== IS NOT A CONCRETE CLASS VIA PCLASS ===

{ "foo": "yes", "__pclass" : { "$binary": "TW9uZ29EQlxCU09OXFVuc2VyaWFsaXphYmxl", "$type": "80" } }
object(YourClass)#%d (3) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(MongoDB\BSON\Binary)#%d (2) {
    ["data"]=>
    string(27) "MongoDB\BSON\Unserializable"
    ["type"]=>
    int(128)
  }
  ["unserialized"]=>
  bool(true)
}


=== PCLASS OVERRIDES TYPEMAP (1) ===

{ "foo": "yes", "__pclass" : { "$binary": "TXlDbGFzcw==", "$type": "80" } }
object(YourClass)#%d (3) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(MongoDB\BSON\Binary)#%d (2) {
    ["data"]=>
    string(7) "MyClass"
    ["type"]=>
    int(128)
  }
  ["unserialized"]=>
  bool(true)
}

{ "foo": "yes", "__pclass" : { "$binary": "T3VyQ2xhc3M=", "$type": "80" } }
object(OurClass)#%d (3) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(MongoDB\BSON\Binary)#%d (2) {
    ["data"]=>
    string(8) "OurClass"
    ["type"]=>
    int(128)
  }
  ["unserialized"]=>
  bool(true)
}

{ "foo": "yes", "__pclass" : { "$binary": "VGhlaXJDbGFzcw==", "$type": "80" } }
object(TheirClass)#%d (3) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(MongoDB\BSON\Binary)#%d (2) {
    ["data"]=>
    string(10) "TheirClass"
    ["type"]=>
    int(128)
  }
  ["unserialized"]=>
  bool(true)
}

{ "foo": "yes", "__pclass" : { "$binary": "WW91ckNsYXNz", "$type": "80" } }
object(YourClass)#%d (3) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(MongoDB\BSON\Binary)#%d (2) {
    ["data"]=>
    string(9) "YourClass"
    ["type"]=>
    int(128)
  }
  ["unserialized"]=>
  bool(true)
}


=== PCLASS OVERRIDES TYPEMAP (2) ===

{ "foo": "yes", "__pclass" : { "$binary": "VGhlaXJDbGFzcw==", "$type": "80" } }
object(TheirClass)#%d (3) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(MongoDB\BSON\Binary)#%d (2) {
    ["data"]=>
    string(10) "TheirClass"
    ["type"]=>
    int(128)
  }
  ["unserialized"]=>
  bool(true)
}


=== OBJECTS AS ARRAY ===

{ "foo": "yes", "bar" : false }
array(2) {
  ["foo"]=>
  string(3) "yes"
  ["bar"]=>
  bool(false)
}

{ "foo": "no", "array" : [ 5, 6 ] }
array(2) {
  ["foo"]=>
  string(2) "no"
  ["array"]=>
  array(2) {
    [0]=>
    int(5)
    [1]=>
    int(6)
  }
}

{ "foo": "no", "obj" : { "embedded" : 4.125 } }
array(2) {
  ["foo"]=>
  string(2) "no"
  ["obj"]=>
  array(1) {
    ["embedded"]=>
    float(4.125)
  }
}

{ "foo": "yes", "__pclass": "MyClass" }
array(2) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  string(7) "MyClass"
}

{ "foo": "yes", "__pclass" : { "$binary": "TXlDbGFzcw==", "$type": "80" } }
array(2) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(MongoDB\BSON\Binary)#%d (2) {
    ["data"]=>
    string(7) "MyClass"
    ["type"]=>
    int(128)
  }
}

{ "foo": "yes", "__pclass" : { "$binary": "T3VyQ2xhc3M=", "$type": "80" } }
array(2) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(MongoDB\BSON\Binary)#%d (2) {
    ["data"]=>
    string(8) "OurClass"
    ["type"]=>
    int(128)
  }
}


=== OBJECTS AS STDCLASS ===

{ "foo": "yes", "__pclass" : { "$binary": "TXlDbGFzcw==", "$type": "80" } }
object(stdClass)#%d (2) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(MongoDB\BSON\Binary)#%d (2) {
    ["data"]=>
    string(7) "MyClass"
    ["type"]=>
    int(128)
  }
}

{ "foo": "yes", "__pclass" : { "$binary": "T3VyQ2xhc3M=", "$type": "80" } }
object(stdClass)#%d (2) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(MongoDB\BSON\Binary)#%d (2) {
    ["data"]=>
    string(8) "OurClass"
    ["type"]=>
    int(128)
  }
}


===DONE===
