--TEST--
BSON encoding: Encoding data into BSON representation, and BSON into Extended JSON
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
use MongoDB\BSON as BSON;

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
    protected $_lastModified;
    protected $_created;
 
    function __construct($username, $email, $name) {
        $this->username = $username;
        $this->email    = $email;
        $this->setName($name);
 
        /* Pregenerate our ObjectID */
        $this->_id     = new BSON\ObjectID();
    }
    function addAddress(Address $address) {
        $this->addresses[] = $address;
    }
 
    function bsonUnserialize(array $array) {
        $this->__original = $array;
        foreach($array as $k => $v) {
            $this->{$k} = $v;
        }
    }
 
    function bsonSerialize() {
        $props = get_object_vars($this);
 
        /* If __original doesn't exist, this is a fresh object that needs to be inserted */
        if (empty($this->__original)) {
            $props["_created"] = new BSON\UTCDateTime(microtime(true) * 1000);
            return $props;
        }
 
        /* Track the last time this person was updated */
        $update = array(
            '$currentDate' => array(
                "_lastModified" => array('$type' => 'date'),
            ),
            '$set' => array(),
        );
        /* Otherwise, only pluck out the changes so we don't have to do full object replacement */
        foreach($props as $k => $v) {
            if (!isset($this->__original[$k])) {
                /* A new field -- likely a on-the-fly schema upgrade */
                $updated['$set'] = array($k => $v);
            }
        }
        return $updated;
    }
 
    function getName() {
        return $this->name;
    }
    function setName($name) {
        return $this->name = $name;
    }
}
 
 
$lair77 = new Person('lair77', 'claire77@example.net', 'Claire Corwin');
$hillardhaven = new Address('4527 Kohler Square Apt. 316', 'Hillardhaven', '02622-5175');
$lair77->addAddress($hillardhaven);
$prudencemouth = new Address('7042 Freida Springs', 'Prudencemouth', '94805');
$lair77->addAddress($prudencemouth);
 
 
$tabitha = new Person('tabitha.mohr', 'mohr@example.org', 'Tabitha Lehner');
$konopelskichester = new Address('76650 Mina Pass', 'Konopelskichester', '69679-5471');
$tabitha->addAddress($konopelskichester);
 
$hartmann = new Person('hartmann', 'hartmann@example.org', 'Hartmann Dedrick');
$leannefurt = new Address('151 Delbert Hills Suite 923', 'Leannefurt', '22036');
$hartmann->addAddress($leannefurt);
 
$ena = new Person('ena', 'ena.sanford@example.net', 'Frida Sanford'); 
$lawsonport = new Address('3656 Jenifer Field', 'New Lawsonport', '16300');
$ena->addAddress($lawsonport);
 
 
$lockman = new Person('cartwright.garland', 'lockman.olga@example.net', 'Alice Carter');
$herminia = new Address('37413 Kailee Spurs', 'East Herminia', '22107');
$lockman->addAddress($herminia);
 
 
$dropcoll = new MongoDB\Driver\Command(array("drop" => COLLECTION_NAME));
$query = new MongoDB\Driver\Query(array());
$hartmannFilter = array("username" => "hartmann");
$queryHartmann = new MongoDB\Driver\Query($hartmannFilter);
 
 
$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert($lair77);
$bulk->insert($tabitha);
$bulk->insert($hartmann);
$bulk->insert($ena);
$bulk->insert($lockman);
 
$m = new MongoDB\Driver\Manager(STANDALONE);
try {
    $m->executeCommand(DATABASE_NAME, $dropcoll);
} catch(Exception $e) {}
 
$m->executeBulkWrite(NS, $bulk);
 
 
foreach($m->executeQuery(NS, $query) as $person) {
    var_dump($person->getName());
}
 
echo "-----\n";
 
$array = $m->executeQuery(NS, $queryHartmann)->toArray();
$hartmann = $array[0];
var_dump($hartmann->getName());
$hartmann->setName("Dr. " . $hartmann->getName());
 
$bulk = new MongoDB\Driver\BulkWrite();
$bulk->update($hartmannFilter, $hartmann);
$retval = $m->executeBulkWrite(NS, $bulk);
var_dump($retval->getModifiedCount());
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(13) "Claire Corwin"
string(14) "Tabitha Lehner"
string(16) "Hartmann Dedrick"
string(13) "Frida Sanford"
string(12) "Alice Carter"
-----
string(16) "Hartmann Dedrick"
int(1)
===DONE===
