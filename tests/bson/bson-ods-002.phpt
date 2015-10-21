--TEST--
BSON encoding: Encoding data into BSON representation, and BSON into Extended JSON
--INI--
date.timezone=America/Los_Angeles
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

    function getStreetAddress()  {
        return $this->streetAddress;
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

        /* These aren't inserted/updated as values */
        unset($props["__original"]);
        unset($props["_lastModified"]);

        /* If __original doesn't exist, this is a fresh object that needs to be inserted */
        if (empty($this->__original)) {
            /* Generate the `_created` timestamp */
            $props["_created"] = new BSON\UTCDateTime(microtime(true) * 1000);
            return $props;
        }

        /* Track the last time this person was updated */
        $updated = array(
            '$currentDate' => array(
                "_lastModified" => array('$type' => "date"),
            ),
            '$set' => array(),
        );

        /* Otherwise, only pluck out the changes so we don't have to do full object replacement */
        foreach($props as $k => $v) {
            if (!isset($this->__original[$k])) {
                /* A new field -- likely a on-the-fly schema upgrade */
                $updated['$set'] = array($k => $v);
                continue;
            }

            /* Changed value */
            if ($this->__original[$k] != $v) {
                $updated['$set'] = array($k => $v);
                continue;
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
    function getCreatedDateTime() {
        return $this->_created->toDateTime();
    }
    function getLastModifiedDateTime() {
        return $this->_lastModified->toDateTime();
    }
    function toArray() {
        $props = get_object_vars($this);

        unset($props["__original"]);

        return $props;
    }
    function getAddresses() {
        return $this->addresses;
    }
}


$m = new MongoDB\Driver\Manager(STANDALONE);

try {
    /* Drop the collection between runs */
    $dropcoll = new MongoDB\Driver\Command(array("drop" => COLLECTION_NAME));
    $m->executeCommand(DATABASE_NAME, $dropcoll);
} catch(Exception $e) {}

$bulk = new MongoDB\Driver\BulkWrite;


/* Claire Corwin, username: lair77 */
$lair77 = new Person("lair77", "claire77@example.net", "Claire Corwin");
$hillardhaven = new Address("4527 Kohler Square Apt. 316", "Hillardhaven", "02622-5175");
$lair77->addAddress($hillardhaven);
$bulk->insert($lair77);


/* Tabitha Lehner, username: tabitha.mohr */
$tabitha = new Person("tabitha.mohr", "mohr@example.org", "Tabitha Lehner");
$konopelskichester = new Address("76650 Mina Pass", "Konopelskichester", "69679-5471");
$tabitha->addAddress($konopelskichester);
$bulk->insert($tabitha);


/* Hartmann Dedrick, username: hartmann -- he is going to be our demo person */
$hartmann = new Person("hartmann", "hartmann@example.org", "Hartmann Dedrick");
$leannefurt = new Address("151 Delbert Hills Suite 923", "Leannefurt", "22036");
$hartmann->addAddress($leannefurt);
/* He has two addresses */
$prudencemouth = new Address("7042 Freida Springs", "Prudencemouth", "94805");
$hartmann->addAddress($prudencemouth);
$bulk->insert($hartmann);


/* Frida Sanford, username: ena */
$ena = new Person("ena", "sanford@example.net", "Frida Sanford"); 
$lawsonport = new Address("3656 Jenifer Field", "New Lawsonport", "16300");
$ena->addAddress($lawsonport);
$bulk->insert($ena);


/* Alice Lockman, username: lockman */
$lockman = new Person("lockman", "lockman.alice@example.net", "Alice Lockman");
$herminia = new Address("37413 Kailee Spurs", "East Herminia", "22107");
$lockman->addAddress($herminia);
$bulk->insert($lockman);


/* Insert our fixtures in one bulk write operation */
$m->executeBulkWrite(NS, $bulk);




/* Iterate over all the `examples.people`, 
 * converting them back into Person objects -- and the nested Address object!
 */
$query = new MongoDB\Driver\Query(array());
foreach($m->executeQuery(NS, $query) as $person) {
    echo $person->getName(), " has the following address(es):\n";
    foreach($person->getAddresses() as $address) {
        echo "\t", $address->getStreetAddress(), "\n";
    }
    echo "\n";
}

echo "-----\n";

/* Hartmann graduated with a doctorate and deserves a Dr. prefix */
$hartmannFilter = array("username" => "hartmann");
$queryHartmann = new MongoDB\Driver\Query($hartmannFilter);
$array = $m->executeQuery(NS, $queryHartmann)->toArray();
$hartmann = $array[0];
$hartmann->setName("Dr. " . $hartmann->getName());

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->update($hartmannFilter, $hartmann);
$retval = $m->executeBulkWrite(NS, $bulk);
printf("Updated %d person (%s)\n", $retval->getModifiedCount(), $hartmann->getName());

$queryAll = new MongoDB\Driver\Query(array());
$all = $m->executeQuery(NS, $queryAll)->toArray();
foreach($all as $person) {
    if ($person->getName() == "Dr. Hartmann Dedrick") {
        $array = $person->toArray();
        var_dump($array["username"]);
        var_dump($person->getCreatedDateTime()->format(DATE_RSS));
        var_dump($person->getLastModifiedDateTime()->format(DATE_RSS));
    }
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Claire Corwin has the following address(es):
	4527 Kohler Square Apt. 316

Tabitha Lehner has the following address(es):
	76650 Mina Pass

Hartmann Dedrick has the following address(es):
	151 Delbert Hills Suite 923
	7042 Freida Springs

Frida Sanford has the following address(es):
	3656 Jenifer Field

Alice Lockman has the following address(es):
	37413 Kailee Spurs

-----
Updated 1 person (Dr. Hartmann Dedrick)
string(8) "hartmann"
string(%d) "%s"
string(%d) "%s"
===DONE===
