# Object serialization

This driver includes four public interfaces to facilitate serializing PHP
classes to and from BSON.

- MongoDB\BSON\Type
    * (no methods)
- MongoDB\BSON\Serializable
    * abstract public method bsonSerialize();
- MongoDB\BSON\Unserializable
    * abstract public method bsonUnserialize(array $data);
- MongoDB\BSON\Persistable
    * extends both BSON\Serializable and BSON\Unserializable

Objects that implement the MongoDB\BSON\Type interface get special treatment by
the BSON serializer. In general, these objects represent a BSON type that cannot
be natively represented in PHP (e.g. MongoDB\BSON\UTCDatetime,
MongoDB\BSON\ObjectID) and are explicitly checked for and handled by the driver.
MongoDB\BSON\Type should not be implemented directly by userland classes.

Userland classes that require custom BSON serialization should utilize the
MongoDB\BSON\Serializable interface and implement the `bsonSerialize()`
function, which should return a document (i.e. PHP array or stdClass object)
representing the document that should be stored. Furthermore, if the object
implements the MongoDB\BSON\Persistable interface, the driver will also inject a
MongoDB\BSON\Binary value (with type 0x80 and an internal field name) into the
document, which contains the userland object's fully qualified classname. This
field can then be used during unserialization to ensure that the BSON document
becomes an object of the same class on the way out of the database.

During unserialization of a document, if a BSON Binary value (with type 0x80 and
within the expected internal field name) is encountered, the driver will peek at
the value and attempt to resolve it to a classname (triggering autoloaders if
neccesary). If the class name cannot be resolved, nothing magical happens;
however, if the class exists, the driver will create a new instance (without
invoking the constructor) and invoke its `bsonUnserialize()` method, supplying
the unserialized document data as a PHP array. Note that this happens
depth-first, which means that embedded documents will be resolved before their
parent.

## Examples

Consider the following two classes: Person and Address. A Person has name, age,
friends (array of Person objects), and addresses (array of Address objects). A
Person also has a secret, that we would rather not store in a database.

```
<?php

class Person implements MongoDB\BSON\Persistable
{
    protected $_id;
    protected $name;
    protected $age;
    protected $address = [];
    protected $friends = [];
    protected $secret = "none";

    function __construct($name, $age)
    {
        $this->name    = $name;
        $this->age     = $age;
        $this->address = [];
        $this->secret  = "$name confidential info";
        /* Pregenerate our ObjectID */
        $this->_id     = new MongoDB\BSON\ObjectID();
    }

    function addAddress(Address $address)
    {
        $this->address[] = $address;
    }

    function addFriend(Person $friend)
    {
        $this->friends[] = $friend;
    }

    function bsonSerialize()
    {
        return [
            "_id"     => $this->_id,
            "name"    => $this->name,
            "age"     => $this->age,
            "address" => $this->address,
            "friends" => $this->friends,
        ];
    }

    function bsonUnserialize(array $data)
    {
        $this->_id     = $data["_id"];
        $this->name    = $data["name"];
        $this->age     = $data["age"];
        $this->address = $data["address"];
        $this->friends = $data["friends"];
    }
}

class Address implements MongoDB\BSON\Persistable
{
    protected $zip;
    protected $country;

    function __construct($zip, $country)
    {
        $this->zip = $zip;
        $this->country = $country;
    }

    function bsonSerialize()
    {
        return [
            "zip"     => $this->zip,
            "country" => $this->country,
        ];
    }

    function bsonUnserialize(array $data)
    {
        $this->zip = $data["zip"];
        $this->country = $data["country"];
    }
}

?>
```

Hannes is 31 years old and has two addresses;

- Sunnyvale, USA
- Kopavogur, Iceland

He is friends with young Jeremy, a 21 year old who lives in Michigan:

```
<?php
$hannes = new Person("Hannes", 31);
$sunnyvale = new Address(94086, "USA");
$kopavogur = new Address(200, "Iceland");
$hannes->addAddress($sunnyvale);
$hannes->addAddress($kopavogur);

$mikola = new Person("Jeremy", 21);
$michigan = new Address(48169, "USA");
$mikola->addAddress($michigan);

$hannes->addFriend($mikola);

?>
```

To save this information, we can insert `$hannes` into the database like so:

```
<?php
try {
    $wc = new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY);
    $manager = new MongoDB\Driver\Manager("mongodb://192.168.112.10:2000");
    $bulk = new MongoDB\Driver\BulkWrite;
    $bulk->insert($hannes);
    $result = $manager->executeBulkWrite("congress.people", $bulk, $wc);
    echo "Hannes has been inserted\n";
} catch(Exception $e) {
    echo $e->getMessage(), "\n";
    exit;
}
?>
```

This will result in the following document stored (as shown by mongo shell):

```
{
  "_id" : ObjectId("551f2004bd21b959de3c15b1"),
  "__pclass" : BinData(128,"UGVyc29u"),
  "name" : "Hannes",
  "age" : 31,
  "address" : [
    {
      "__pclass" : BinData(128,"QWRkcmVzcw=="),
      "zip" : 94086,
      "country" : "USA"
    },
    {
      "__pclass" : BinData(128,"QWRkcmVzcw=="),
      "zip" : 200,
      "country" : "Iceland"
    }
  ],
  "friends" : [
    {
      "__pclass" : BinData(128,"UGVyc29u"),
      "_id" : ObjectId("551f2004bd21b959de3c15b2"),
      "name" : "Jeremy",
      "age" : 21,
      "address" : [
        {
          "__pclass" : BinData(128,"QWRkcmVzcw=="),
          "zip" : 48169,
          "country" : "USA"
        }
      ],
      "friends" : {
        
      }
    }
  ]
}
```

When we read this document back, the driver will convert the individual
sub-documents back into their original classes, as we have full type information
for them now:

```
<?php

$rp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY_PREFERRED);
$query = new MongoDB\Driver\Query([]);
$cursor = $manager->executeQuery("congress.people", $query, $rp);

foreach($cursor as $person) {
    var_dump($person);
}

?>
```

This results in:

```
object(Person)#22 (6) {
  ["_id":protected]=>
  object(BSON\ObjectID)#15 (0) {
  }
  ["name":protected]=>
  string(6) "Hannes"
  ["age":protected]=>
  int(31)
  ["address":protected]=>
  array(2) {
    [0]=>
    object(Address)#16 (2) {
      ["zip":protected]=>
      int(94086)
      ["country":protected]=>
      string(3) "USA"
    }
    [1]=>
    object(Address)#17 (2) {
      ["zip":protected]=>
      int(200)
      ["country":protected]=>
      string(7) "Iceland"
    }
  }
  ["friends":protected]=>
  array(1) {
    [0]=>
    object(Person)#21 (6) {
      ["_id":protected]=>
      object(BSON\ObjectID)#18 (0) {
      }
      ["name":protected]=>
      string(6) "Jeremy"
      ["age":protected]=>
      int(21)
      ["address":protected]=>
      array(1) {
        [0]=>
        object(Address)#19 (2) {
          ["zip":protected]=>
          int(48169)
          ["country":protected]=>
          string(3) "USA"
        }
      }
      ["friends":protected]=>
      object(stdClass)#20 (0) {
      }
      ["secret":protected]=>
      string(4) "none"
    }
  }
  ["secret":protected]=>
  string(4) "none"
}
```
