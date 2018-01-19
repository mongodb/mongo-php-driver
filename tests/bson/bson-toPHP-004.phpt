--TEST--
MongoDB\BSON\toPHP(): BSON array keys should be disregarded during visitation
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class MyArrayObject extends ArrayObject implements MongoDB\BSON\Unserializable
{
    function bsonUnserialize(array $data)
    {
        parent::__construct($data);
    }
}

$tests = [
    null,
    true,
    1,
    4.125,
    'foo',
    [],
    (object) [],
    new MongoDB\BSON\Binary('foo', MongoDB\BSON\Binary::TYPE_GENERIC),
    new MongoDB\BSON\Decimal128('3.14'),
    new MongoDB\BSON\Javascript('function(){}'),
    new MongoDB\BSON\MaxKey,
    new MongoDB\BSON\MinKey,
    new MongoDB\BSON\ObjectId('586c18d86118fd6c9012dec1'),
    new MongoDB\BSON\Regex('foo'),
    new MongoDB\BSON\Timestamp(1234, 5678),
    new MongoDB\BSON\UTCDateTime('1483479256924'),
];

foreach ($tests as $value) {
    printf("Testing %s visitor function\n", is_object($value) ? get_class($value) : gettype($value));
    $bson = fromPHP(['x' => [$value]]);
    // Alter the key of the BSON array's first element
    $bson[12] = '1';

    var_dump(toPHP($bson));

    /* Note that numeric indexes within the HashTable are not accessible without
     * casting the object to an array. This is because the entries are only
     * stored with numeric indexes and do not also have string equivalents, as
     * might be created with zend_symtable_update(). This behavior is not unique
     * to the driver, as `(object) ['foo']` would demonstrate the same issue. */
    var_dump(toPHP($bson, ['array' => 'object']));

    var_dump(toPHP($bson, ['array' => 'MyArrayObject']));

    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing NULL visitor function
object(stdClass)#%d (1) {
  ["x"]=>
  array(1) {
    [0]=>
    NULL
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(stdClass)#%d (1) {
    [%r(0|"0")%r]=>
    NULL
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(MyArrayObject)#%d (1) {
    ["storage":"ArrayObject":private]=>
    array(1) {
      [0]=>
      NULL
    }
  }
}

Testing boolean visitor function
object(stdClass)#%d (1) {
  ["x"]=>
  array(1) {
    [0]=>
    bool(true)
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(stdClass)#%d (1) {
    [%r(0|"0")%r]=>
    bool(true)
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(MyArrayObject)#%d (1) {
    ["storage":"ArrayObject":private]=>
    array(1) {
      [0]=>
      bool(true)
    }
  }
}

Testing integer visitor function
object(stdClass)#%d (1) {
  ["x"]=>
  array(1) {
    [0]=>
    int(1)
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(stdClass)#%d (1) {
    [%r(0|"0")%r]=>
    int(1)
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(MyArrayObject)#%d (1) {
    ["storage":"ArrayObject":private]=>
    array(1) {
      [0]=>
      int(1)
    }
  }
}

Testing double visitor function
object(stdClass)#%d (1) {
  ["x"]=>
  array(1) {
    [0]=>
    float(4.125)
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(stdClass)#%d (1) {
    [%r(0|"0")%r]=>
    float(4.125)
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(MyArrayObject)#%d (1) {
    ["storage":"ArrayObject":private]=>
    array(1) {
      [0]=>
      float(4.125)
    }
  }
}

Testing string visitor function
object(stdClass)#%d (1) {
  ["x"]=>
  array(1) {
    [0]=>
    string(3) "foo"
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(stdClass)#%d (1) {
    [%r(0|"0")%r]=>
    string(3) "foo"
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(MyArrayObject)#%d (1) {
    ["storage":"ArrayObject":private]=>
    array(1) {
      [0]=>
      string(3) "foo"
    }
  }
}

Testing array visitor function
object(stdClass)#%d (1) {
  ["x"]=>
  array(1) {
    [0]=>
    array(0) {
    }
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(stdClass)#%d (1) {
    [%r(0|"0")%r]=>
    object(stdClass)#%d (0) {
    }
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(MyArrayObject)#%d (1) {
    ["storage":"ArrayObject":private]=>
    array(1) {
      [0]=>
      object(MyArrayObject)#%d (1) {
        ["storage":"ArrayObject":private]=>
        array(0) {
        }
      }
    }
  }
}

Testing stdClass visitor function
object(stdClass)#%d (1) {
  ["x"]=>
  array(1) {
    [0]=>
    object(stdClass)#%d (0) {
    }
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(stdClass)#%d (1) {
    [%r(0|"0")%r]=>
    object(stdClass)#%d (0) {
    }
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(MyArrayObject)#%d (1) {
    ["storage":"ArrayObject":private]=>
    array(1) {
      [0]=>
      object(stdClass)#%d (0) {
      }
    }
  }
}

Testing MongoDB\BSON\Binary visitor function
object(stdClass)#%d (1) {
  ["x"]=>
  array(1) {
    [0]=>
    object(MongoDB\BSON\Binary)#%d (2) {
      ["data"]=>
      string(3) "foo"
      ["type"]=>
      int(0)
    }
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(stdClass)#%d (1) {
    [%r(0|"0")%r]=>
    object(MongoDB\BSON\Binary)#%d (2) {
      ["data"]=>
      string(3) "foo"
      ["type"]=>
      int(0)
    }
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(MyArrayObject)#%d (1) {
    ["storage":"ArrayObject":private]=>
    array(1) {
      [0]=>
      object(MongoDB\BSON\Binary)#%d (2) {
        ["data"]=>
        string(3) "foo"
        ["type"]=>
        int(0)
      }
    }
  }
}

Testing MongoDB\BSON\Decimal128 visitor function
object(stdClass)#%d (1) {
  ["x"]=>
  array(1) {
    [0]=>
    object(MongoDB\BSON\Decimal128)#%d (1) {
      ["dec"]=>
      string(4) "3.14"
    }
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(stdClass)#%d (1) {
    [%r(0|"0")%r]=>
    object(MongoDB\BSON\Decimal128)#%d (1) {
      ["dec"]=>
      string(4) "3.14"
    }
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(MyArrayObject)#%d (1) {
    ["storage":"ArrayObject":private]=>
    array(1) {
      [0]=>
      object(MongoDB\BSON\Decimal128)#%d (1) {
        ["dec"]=>
        string(4) "3.14"
      }
    }
  }
}

Testing MongoDB\BSON\Javascript visitor function
object(stdClass)#%d (1) {
  ["x"]=>
  array(1) {
    [0]=>
    object(MongoDB\BSON\Javascript)#%d (2) {
      ["code"]=>
      string(12) "function(){}"
      ["scope"]=>
      NULL
    }
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(stdClass)#%d (1) {
    [%r(0|"0")%r]=>
    object(MongoDB\BSON\Javascript)#%d (2) {
      ["code"]=>
      string(12) "function(){}"
      ["scope"]=>
      NULL
    }
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(MyArrayObject)#%d (1) {
    ["storage":"ArrayObject":private]=>
    array(1) {
      [0]=>
      object(MongoDB\BSON\Javascript)#%d (2) {
        ["code"]=>
        string(12) "function(){}"
        ["scope"]=>
        NULL
      }
    }
  }
}

Testing MongoDB\BSON\MaxKey visitor function
object(stdClass)#%d (1) {
  ["x"]=>
  array(1) {
    [0]=>
    object(MongoDB\BSON\MaxKey)#%d (0) {
    }
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(stdClass)#%d (1) {
    [%r(0|"0")%r]=>
    object(MongoDB\BSON\MaxKey)#%d (0) {
    }
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(MyArrayObject)#%d (1) {
    ["storage":"ArrayObject":private]=>
    array(1) {
      [0]=>
      object(MongoDB\BSON\MaxKey)#%d (0) {
      }
    }
  }
}

Testing MongoDB\BSON\MinKey visitor function
object(stdClass)#%d (1) {
  ["x"]=>
  array(1) {
    [0]=>
    object(MongoDB\BSON\MinKey)#%d (0) {
    }
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(stdClass)#%d (1) {
    [%r(0|"0")%r]=>
    object(MongoDB\BSON\MinKey)#%d (0) {
    }
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(MyArrayObject)#%d (1) {
    ["storage":"ArrayObject":private]=>
    array(1) {
      [0]=>
      object(MongoDB\BSON\MinKey)#%d (0) {
      }
    }
  }
}

Testing MongoDB\BSON\ObjectId visitor function
object(stdClass)#%d (1) {
  ["x"]=>
  array(1) {
    [0]=>
    object(MongoDB\BSON\ObjectId)#%d (1) {
      ["oid"]=>
      string(24) "586c18d86118fd6c9012dec1"
    }
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(stdClass)#%d (1) {
    [%r(0|"0")%r]=>
    object(MongoDB\BSON\ObjectId)#%d (1) {
      ["oid"]=>
      string(24) "586c18d86118fd6c9012dec1"
    }
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(MyArrayObject)#%d (1) {
    ["storage":"ArrayObject":private]=>
    array(1) {
      [0]=>
      object(MongoDB\BSON\ObjectId)#%d (1) {
        ["oid"]=>
        string(24) "586c18d86118fd6c9012dec1"
      }
    }
  }
}

Testing MongoDB\BSON\Regex visitor function
object(stdClass)#%d (1) {
  ["x"]=>
  array(1) {
    [0]=>
    object(MongoDB\BSON\Regex)#%d (2) {
      ["pattern"]=>
      string(3) "foo"
      ["flags"]=>
      string(0) ""
    }
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(stdClass)#%d (1) {
    [%r(0|"0")%r]=>
    object(MongoDB\BSON\Regex)#%d (2) {
      ["pattern"]=>
      string(3) "foo"
      ["flags"]=>
      string(0) ""
    }
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(MyArrayObject)#%d (1) {
    ["storage":"ArrayObject":private]=>
    array(1) {
      [0]=>
      object(MongoDB\BSON\Regex)#%d (2) {
        ["pattern"]=>
        string(3) "foo"
        ["flags"]=>
        string(0) ""
      }
    }
  }
}

Testing MongoDB\BSON\Timestamp visitor function
object(stdClass)#%d (1) {
  ["x"]=>
  array(1) {
    [0]=>
    object(MongoDB\BSON\Timestamp)#%d (2) {
      ["increment"]=>
      string(4) "1234"
      ["timestamp"]=>
      string(4) "5678"
    }
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(stdClass)#%d (1) {
    [%r(0|"0")%r]=>
    object(MongoDB\BSON\Timestamp)#%d (2) {
      ["increment"]=>
      string(4) "1234"
      ["timestamp"]=>
      string(4) "5678"
    }
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(MyArrayObject)#%d (1) {
    ["storage":"ArrayObject":private]=>
    array(1) {
      [0]=>
      object(MongoDB\BSON\Timestamp)#%d (2) {
        ["increment"]=>
        string(4) "1234"
        ["timestamp"]=>
        string(4) "5678"
      }
    }
  }
}

Testing MongoDB\BSON\UTCDateTime visitor function
object(stdClass)#%d (1) {
  ["x"]=>
  array(1) {
    [0]=>
    object(MongoDB\BSON\UTCDateTime)#%d (1) {
      ["milliseconds"]=>
      string(13) "1483479256924"
    }
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(stdClass)#%d (1) {
    [%r(0|"0")%r]=>
    object(MongoDB\BSON\UTCDateTime)#%d (1) {
      ["milliseconds"]=>
      string(13) "1483479256924"
    }
  }
}
object(stdClass)#%d (1) {
  ["x"]=>
  object(MyArrayObject)#%d (1) {
    ["storage":"ArrayObject":private]=>
    array(1) {
      [0]=>
      object(MongoDB\BSON\UTCDateTime)#%d (1) {
        ["milliseconds"]=>
        string(13) "1483479256924"
      }
    }
  }
}

===DONE===
