<?php

class Person implements BsonSerializable {
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

class Address implements BsonSerializable {
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

class Country implements BsonSerializable {
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
