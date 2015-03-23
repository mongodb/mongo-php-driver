# Object serialization

pecl/mongodb exports 4 public interfaces to facilitate serializing PHP classes to and from BSON.

- BSON\Type
    * (no methods)
- BSON\Serializable
    * abstract public method bsonSerialize();
- BSON\Unserializable
    * abstract public method bsonUnserialize(array $data);
- BSON\Persistable
    * implements both BSON\Serializable and BSON\Unserializable


Objects that implement the BSON\Type interface get very special treatment by the BSON serializer. In
general, these objects represent a BSON type that cannot be natively represented in PHP - such as
BSON\UTCDatetime and BSON\ObjectID - and are specifically checked for and handled.
Note that implmenting BSON\Type and or BSON\Unserializable standalone does not do anything for
userland classes.

Since BSON\Type is a public interface, normal userland classes can indeed implement it to, and apply
their own custom rules on the serialization.
To achieve this though, the BSON\Serializable interface should be implemented instead, providing the
bsonSerialize() function which should return a PHP array() representing the document that should be
stored.
Furthermore, if the object implements the BSON\Persistable interface, the driver will embed a BSON
Binary (0x80) with the fully qualified classname, into the document.

During unserialization of a document, if a BSON Binary (of type 0x80) is encountered, the driver
will peak at the value and attempt to resolve it to a classname (triggering autoloaders if
neccesary). If the classname cannot be resolved, no harm no faul, and nothing magical happens.
However, if the class exists, the driver will call the bsonUnserialize() method of the detected
class, passing it the full document.
Note that this happens depth-first, meaning any subdocuments (relations) are resolved first, and
then crawled upwards to the start of the document.


## Examples

Imagine the following two classes, Person and Address.

A Person has name, age, friends (Person), and addresses (Address).
A Person also has a secret, that we would rather not store in a database.

```
<?php
class Person implements BSON\Persistable {
    protected $_id;
    protected $name;
    protected $age;
    protected $address = array();
    protected $friends = array();
    protected $secret = "none";

    function __construct($name, $age) {
        $this->name    = $name;
        $this->age     = $age;
        $this->address = array();
        $this->secret  = "$name confidential info";
        /* Pregenerate our ObjectID */
        $this->_id     = new BSON\ObjectID();
    }
    function addAddress(Address $address) {
        $this->address[] = $address;
    }
    function addFriend(Person $friend) {
        $this->friends[] = $friend;
    }
    function bsonSerialize() {
        return array(
            "_id"     => $this->_id,
            "name"    => $this->name,
            "age"     => $this->age,
            "address" => $this->address,
            "friends" => $this->friends,
        );
    }
    function bsonUnserialize(array $data) {
        $this->_id     = $data["_id"];
        $this->name    = $data["name"];
        $this->age     = $data["age"];
        $this->address = $data["address"];
        $this->friends = $data["friends"];
    }
}

class Address implements BSON\Persistable {
    protected $zip;
    protected $country;

    function __construct($zip, $country) {
        $this->zip = $zip;
        $this->country = $country;
    }
    function bsonSerialize() {
        return array(
            "zip"     => $this->zip,
            "country" => $this->country,
        );
    }
    function bsonUnserialize(array $data) {
        $this->zip = $data["zip"];
        $this->country = $data["country"];
    }
}
?>
```

Hannes, 31 years old, has two addresses;

- Sunnyvale, USA
- Kopavogur, Iceland

He is friends with young Jeremy, 21 year old who lives in Michigan

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


To save this information we can insert $hannes to the database like so:

```
<?php
try {
    $wc = new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY);
    $manager = new MongoDB\Driver\Manager("mongodb://192.168.112.10:2000");
    $result = $manager->executeInsert("congress.people", $hannes, $wc);
    echo "Hannes has been inserted\n";
} catch(Exception $e) {
    echo $e->getMessage(), "\n";
    exit;
}
?>
```

This will result in the following document to be stored (as shown by mongo shell):

```
{
        "_id" : ObjectId("54c9664fbd21b9416f2e0501"),
        "__" : BinData(128,"UGVyc29u"),
        "name" : "Hannes",
        "age" : 31,
        "address" : [
                [
                        BinData(128,"QWRkcmVzcw=="),
                        94086,
                        "USA"
                ],
                [
                        BinData(128,"QWRkcmVzcw=="),
                        200,
                        "Iceland"
                ]
        ],
        "friends" : [
                [
                        BinData(128,"UGVyc29u"),
                        ObjectId("54c9664fbd21b9416f2e0502"),
                        "Jeremy",
                        21,
                        [
                                [
                                        BinData(128,"QWRkcmVzcw=="),
                                        48169,
                                        "USA"
                                ]
                        ],
                        {

                        }
                ]
        ]
}
```

When we read this document back, the driver will convert the individual subdocuments back into the
original classes, as we have full type information for them now:

```
<?php
$rp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY_PREFERRED);
$query = new MongoDB\Driver\Query(array());
$cursor = $manager->executeQuery("congress.people", $query, $rp);
foreach($cursor as $person) {
	var_dump($person);
}
?>
```

Which results in:

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
