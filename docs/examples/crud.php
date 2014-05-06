<?php
class Person implements Mongolizable {
    protected $name;
    protected $nick;
    protected $addresses = array();

    function __construct($name, $nick, array $addresses) {
        $this->name = $name;
        $this->nick = $nick;
        foreach($addresses as $address) {
            if ($address instanceof Address) {
                $this->addresses[] = $address;
            }
        }
    }

    function getAddresses() {
        return $this->addresses;
    }
}
class Address implements Mongolizable {
    protected $address;
    protected $country;

    function __construct($address, Country $country) {
        $this->address = $address;
        $this->country = $country;
    }
    function getCountry() {
        return $this->country;
    }
}

class Country implements Mongolizable {
    protected $name;

    function __construct($name) {
        $this->name;
    }

    function getName() {
        return $name;
    }
}


$norway  = new Country("Norway");
$iceland = new Country("Iceland");
$usa     = new Country("USA");

$menlo   = new Address("22 Coleman Pl", $usa);

$addresses = array(
    new Address("Dynekilgata 15", $norway),
    new Address("Mánabraut 4", $iceland),
    $menlo,
);

$hannes = new Person("Hannes", "bjori", $addresses);
$hayley = new Person("Hayley", "Ninja", array($menlo));




/* Read/Write general options */
$writeOptions = array(
    "ordered" => true,
    "writeConcern" => array(
        "w" => 3,
        "wtimeout" => 42,
    ),
);
$readPreference = array(
    "type" => \MongoDB\ReadPreference\SECONDARY_PREFERRED,
    "tags" => array(
        array("dc:eu", "cpu:fast"),
        array("dc:eu", "cc:is"),
        array(""),
    ),
);




/* Constructing a query */
$query = array(
    '$query'   => array("name" => "Hannes"),
    '$orderBy' => array('name' => 1),
    '$comment' => 'More special stuff',
    '$select'  => array("fieldname" => 1, "fieldname.two" => 1), /* Internally rewritten to wire protocol option */
    '$skip'    => 0,  /* Don't skip any docs */                  /* Internally rewritten to wire protocol option */
    '$limit'   => -3, /* Kill server cursor after 3 docs */      /* Internally rewritten to wire protocol option */
    '$flags'   => EXHAUST | NO_CURSOR_TIMEOUT,                   /* Internally rewritten to wire protocol option */
);

$queryHannes = new Query($query);
$queryUSA    = new Query(array('$query' => array('addresses.$.Country' => "USA")));




/* Main object. Manager instance / MongoClient */
$mm = new MongoDB\Manager("mongodb://server1,server2/?replicaSet=FOOBAR");



$batch = new MongoDB\Batch\Write;
$batch->add($hannes)->add($hayley);

$results = $mm->executeWrite("myproject.people", $batch, $writeOptions);
$results instanceof WriteResults;

echo "Wrote ", $results->getOpCount(), " to ", $results->getServer()->getHostname(), " in ", $results->getTimer(), " ms\n";



$results = $mm->executeQuery("myproject.people", $queryHannes, $readPreference);
$results instanceof QueryResults;



foreach($results as $person) {
    echo $person->getName() , " has lived in ", $person->getAddresses()[0]->getCountry()->getName(), "\n";
}












