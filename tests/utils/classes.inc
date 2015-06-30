<?php
use MongoDB\BSON as BSON;

class Person implements BSON\Persistable {
    protected $name;
    protected $age;
    protected $address = array();
    protected $friends = array();
    protected $secret = "none";

    function __construct($name, $age) {
        $this->name = $name;
        $this->age  = $age;
        $this->address = array();
        $this->secret = "$name confidential info";
    }
    function addAddress(Address $address) {
        $this->address[] = $address;
    }
    function addFriend(Person $friend) {
        $this->friends[] = $friend;
    }
    function bsonSerialize() {
        return array(
            "name"    => $this->name,
            "age"     => $this->age,
            "address" => $this->address,
            "friends" => $this->friends,
        );
    }
    function bsonUnserialize(array $data) {
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


