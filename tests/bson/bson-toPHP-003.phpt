--TEST--
BSON\toPHP(): Tests from serialization specification
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";
use MongoDB\BSON as BSON;

class MyClass
{
}

class YourClass implements BSON\Unserializable
{
    function bsonUnserialize(array $data)
    {
        foreach ($data as $key => $value) {
            $this->$key = $value;
        }
        $this->unserialized = true;
    }
}

class OurClass implements BSON\Persistable
{
    function bsonSerialize()
    {
        // Not tested with this test, so return empty array
        return array();
    }

    function bsonUnserialize(array $data)
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
$bInterface = base64_encode(BSON_NAMESPACE . '\Unserializable');

$testGroups = array(
    array(
        'name' => 'DEFAULT TYPEMAP',
        'typemap' => array(),
        'tests' => array(
            '{ "foo": "yes", "bar" : false }',
            '{ "foo": "no", "array" : [ 5, 6 ] }',
            '{ "foo": "no", "obj" : { "embedded" : 3.14 } }',
            '{ "foo": "yes", "__pclass": "MyClass" }',
            '{ "foo": "yes", "__pclass": { "$type" : "80", "$binary" : "' . $bMyClass . '" } }',
            '{ "foo": "yes", "__pclass": { "$type" : "80", "$binary" : "' . $bYourClass . '" } }',
            '{ "foo": "yes", "__pclass": { "$type" : "80", "$binary" : "' . $bOurClass . '" } }',
            '{ "foo": "yes", "__pclass": { "$type" : "44", "$binary" : "' . $bYourClass . '" } }',
        ),
    ),
    array(
        'name' => 'NONEXISTING CLASS',
        'typemap' => array('root' => 'MissingClass'),
        'tests' => array(
            '{ "foo": "yes" }',
        ),
    ),
    array(
        'name' => 'DOES NOT IMPLEMENT UNSERIALIZABLE',
        'typemap' => array('root' => 'MyClass'),
        'tests' => array(
            '{ "foo": "yes", "__pclass": { "$type": "80", "$binary": "' . $bMyClass . '" } }',
        ),
    ),
    array(
        'name' => 'IS NOT A CONCRETE CLASS',
        'typemap' => array('root' => 'MongoDB\BSON\Unserializable'),
        'tests' => array(
            '{ "foo": "yes" }',
        ),
    ),
    array(
        'name' => 'IS NOT A CONCRETE CLASS VIA PCLASS',
        'typemap' => array('root' => 'YourClass'),
        'tests' => array(
            '{ "foo": "yes", "__pclass" : { "$type": "80", "$binary": "' . $bInterface . '" } }',
        ),
    ),
    array(
        'name' => 'PCLASS OVERRIDES TYPEMAP (1)',
        'typemap' => array('root' => 'YourClass'),
        'tests' => array(
            '{ "foo": "yes", "__pclass" : { "$type": "80", "$binary": "' . $bMyClass . '" } }',
            '{ "foo": "yes", "__pclass" : { "$type": "80", "$binary": "' . $bOurClass . '" } }',
            '{ "foo": "yes", "__pclass" : { "$type": "80", "$binary": "' . $bTheirClass . '" } }',
            '{ "foo": "yes", "__pclass" : { "$type": "80", "$binary": "' . $bYourClass . '" } }',
        ),
    ),
    array(
        'name' => 'PCLASS OVERRIDES TYPEMAP (2)',
        'typemap' => array('root' => 'OurClass'),
        'tests' => array(
            '{ "foo": "yes", "__pclass" : { "$type": "80", "$binary": "' . $bTheirClass . '" } }',
        ),
    ),
    array(
        'name' => 'OBJECTS AS ARRAY',
        'typemap' => array('root' => 'array', 'document' => 'array'),
        'tests' => array(
            '{ "foo": "yes", "bar" : false }',
            '{ "foo": "no", "array" : [ 5, 6 ] }',
            '{ "foo": "no", "obj" : { "embedded" : 3.14 } }',
            '{ "foo": "yes", "__pclass": "MyClass" }',
            '{ "foo": "yes", "__pclass" : { "$type": "80", "$binary": "' . $bMyClass . '" } }',
            '{ "foo": "yes", "__pclass" : { "$type": "80", "$binary": "' . $bOurClass . '" } }',
        ),
    ),
    array(
        'name' => 'OBJECTS AS STDCLASS',
        'typemap' => array('root' => 'object', 'document' => 'object'),
        'tests' => array(
            '{ "foo": "yes", "__pclass" : { "$type": "80", "$binary": "' . $bMyClass . '" } }',
            '{ "foo": "yes", "__pclass" : { "$type": "80", "$binary": "' . $bOurClass . '" } }',
        ),
    ),
);

foreach ($testGroups as $testGroup) {
    printf("=== %s ===\n\n", $testGroup['name']);

    foreach ($testGroup['tests'] as $test) {
        echo $test, "\n";

        $bson = fromJSON($test);
        try {
            var_dump(toPHP($bson, $testGroup['typemap']));
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

{ "foo": "no", "obj" : { "embedded" : 3.14 } }
object(stdClass)#%d (2) {
  ["foo"]=>
  string(2) "no"
  ["obj"]=>
  object(stdClass)#%d (1) {
    ["embedded"]=>
    float(3.14)
  }
}

{ "foo": "yes", "__pclass": "MyClass" }
object(stdClass)#%d (2) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  string(7) "MyClass"
}

{ "foo": "yes", "__pclass": { "$type" : "80", "$binary" : "TXlDbGFzcw==" } }
object(stdClass)#%d (2) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(%SBSON\Binary)#%d (2) {
    ["data"]=>
    string(7) "MyClass"
    ["type"]=>
    int(128)
  }
}

{ "foo": "yes", "__pclass": { "$type" : "80", "$binary" : "WW91ckNsYXNz" } }
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

{ "foo": "yes", "__pclass": { "$type" : "80", "$binary" : "T3VyQ2xhc3M=" } }
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

{ "foo": "yes", "__pclass": { "$type" : "44", "$binary" : "WW91ckNsYXNz" } }
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

{ "foo": "yes", "__pclass": { "$type": "80", "$binary": "TXlDbGFzcw==" } }
Class MyClass does not implement MongoDB\BSON\Unserializable


=== IS NOT A CONCRETE CLASS ===

{ "foo": "yes" }
Class %SBSON\Unserializable is not instantiatable


=== IS NOT A CONCRETE CLASS VIA PCLASS ===

{ "foo": "yes", "__pclass" : { "$type": "80", "$binary": "TW9uZ29EQlxCU09OXFVuc2VyaWFsaXphYmxl" } }
object(YourClass)#%d (3) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(%SBSON\Binary)#%d (2) {
    ["data"]=>
    string(27) "%SBSON\Unserializable"
    ["type"]=>
    int(128)
  }
  ["unserialized"]=>
  bool(true)
}


=== PCLASS OVERRIDES TYPEMAP (1) ===

{ "foo": "yes", "__pclass" : { "$type": "80", "$binary": "TXlDbGFzcw==" } }
object(YourClass)#%d (3) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(%SBSON\Binary)#%d (2) {
    ["data"]=>
    string(7) "MyClass"
    ["type"]=>
    int(128)
  }
  ["unserialized"]=>
  bool(true)
}

{ "foo": "yes", "__pclass" : { "$type": "80", "$binary": "T3VyQ2xhc3M=" } }
object(OurClass)#%d (3) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(%SBSON\Binary)#%d (2) {
    ["data"]=>
    string(8) "OurClass"
    ["type"]=>
    int(128)
  }
  ["unserialized"]=>
  bool(true)
}

{ "foo": "yes", "__pclass" : { "$type": "80", "$binary": "VGhlaXJDbGFzcw==" } }
object(TheirClass)#%d (3) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(%SBSON\Binary)#%d (2) {
    ["data"]=>
    string(10) "TheirClass"
    ["type"]=>
    int(128)
  }
  ["unserialized"]=>
  bool(true)
}

{ "foo": "yes", "__pclass" : { "$type": "80", "$binary": "WW91ckNsYXNz" } }
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

{ "foo": "yes", "__pclass" : { "$type": "80", "$binary": "VGhlaXJDbGFzcw==" } }
object(TheirClass)#%d (3) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(%SBSON\Binary)#%d (2) {
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

{ "foo": "no", "obj" : { "embedded" : 3.14 } }
array(2) {
  ["foo"]=>
  string(2) "no"
  ["obj"]=>
  array(1) {
    ["embedded"]=>
    float(3.14)
  }
}

{ "foo": "yes", "__pclass": "MyClass" }
array(2) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  string(7) "MyClass"
}

{ "foo": "yes", "__pclass" : { "$type": "80", "$binary": "TXlDbGFzcw==" } }
array(2) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(%SBSON\Binary)#%d (2) {
    ["data"]=>
    string(7) "MyClass"
    ["type"]=>
    int(128)
  }
}

{ "foo": "yes", "__pclass" : { "$type": "80", "$binary": "T3VyQ2xhc3M=" } }
array(2) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(%SBSON\Binary)#%d (2) {
    ["data"]=>
    string(8) "OurClass"
    ["type"]=>
    int(128)
  }
}


=== OBJECTS AS STDCLASS ===

{ "foo": "yes", "__pclass" : { "$type": "80", "$binary": "TXlDbGFzcw==" } }
object(stdClass)#%d (2) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(%SBSON\Binary)#%d (2) {
    ["data"]=>
    string(7) "MyClass"
    ["type"]=>
    int(128)
  }
}

{ "foo": "yes", "__pclass" : { "$type": "80", "$binary": "T3VyQ2xhc3M=" } }
object(stdClass)#%d (2) {
  ["foo"]=>
  string(3) "yes"
  ["__pclass"]=>
  object(%SBSON\Binary)#%d (2) {
    ["data"]=>
    string(8) "OurClass"
    ["type"]=>
    int(128)
  }
}


===DONE===
