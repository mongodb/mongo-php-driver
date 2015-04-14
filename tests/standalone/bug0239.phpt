--TEST--
PHPC-239: Iterator segfault
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE)?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class Address implements BSON\Persistable {
    protected $streetAddress;
    protected $city;
    protected $postalCode;

    function __construct($streetAddress, $city, $postalCode) {
        $this->streetAddress = $streetAddress;
        $this->city          = $city;
        $this->postalCode    = $postalCode;

    }
    function bsonUnserialize(array $array) {
        foreach($array as $k => $v) {
            $this->{$k} = $v;
        }

        return $this;
    }

    function bsonSerialize() {
        return get_object_vars($this);
    }
}

class Person implements BSON\Persistable {
    protected $_id;
    protected $username;
    protected $email;
    protected $name;
    protected $addresses = array();

    function __construct($username, $email, $name) {
        $this->username = $username;
        $this->email    = $email;
        $this->name     = $name;

        /* Pregenerate our ObjectID */
        $this->_id     = new BSON\ObjectID();
    }
    function addAddress(Address $address) {
        $this->addresses[] = $address;
    }

    function bsonUnserialize(array $array) {
        foreach($array as $k => $v) {
            $this->{$k} = $v;
        }
    }

    function bsonSerialize() {
        return get_object_vars($this);
    }

    function getName() {
        return $this->name;
    }
}


$bulk = new MongoDB\Driver\BulkWrite;


$lair77 = new Person('lair77', 'claire77@example.net', 'Claire Corwin');
$hillardhaven = new Address('4527 Kohler Square Apt. 316', 'Hillardhaven', '02622-5175');
$lair77->addAddress($hillardhaven);
$prudencemouth = new Address('7042 Freida Springs', 'Prudencemouth', '94805');
$lair77->addAddress($prudencemouth);
$bulk->insert($lair77);



$hartmann = new Person('hartmann', 'hartmann@example.org', 'Hartmann Dedrick');
$leannefurt = new Address('151 Delbert Hills Suite 923', 'Leannefurt', '22036');
$hartmann->addAddress($leannefurt);
$bulk->insert($hartmann);


$dropcoll = new MongoDB\Driver\Command(array("drop" => COLLECTION_NAME));
$query = new MongoDB\Driver\Query(array());
$queryHartmann = new MongoDB\Driver\Query(array("username" => "hartmann"));



$m = new MongoDB\Driver\Manager(STANDALONE);
try {
    $m->executeCommand(DATABASE_NAME, $dropcoll);
} catch(Exception $e) {}

$m->executeBulkWrite(NS, $bulk);


$res = $m->executeQuery(NS, $query);
foreach($res as $person) {
    var_dump($person->getName());
}
foreach($res as $person) {
    echo "This will never happen: ";
    echo $person->getName(), "\n";
}

echo "-----\n";
$cursor = $m->executeQuery(NS, $queryHartmann);
foreach($cursor as $hartmann) {
    echo $hartmann->getName(), "\n";
}

foreach($cursor as $hartmann) {
    echo "This will never happen: ";
    echo $hartmann->getName(), "\n";
}
foreach($cursor as $hartmann) {
    echo "This will never happen: ";
    echo $hartmann->getName(), "\n";
}
echo "All Good!\n";
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(13) "Claire Corwin"
string(16) "Hartmann Dedrick"
-----
Hartmann Dedrick
All Good!
===DONE===
