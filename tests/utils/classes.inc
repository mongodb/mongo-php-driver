<?php

class Person implements MongoDB\BSON\Persistable {
    protected $name;
    protected $age;
    protected $addresses = array();
    protected $friends = array();
    protected $secret = "none";

    public function __construct($name, $age) {
        $this->name = $name;
        $this->age  = $age;
        $this->addresses = array();
        $this->secret = "$name confidential info";
    }
    public function addAddress(Address $address) {
        $this->addresses[] = $address;
    }
    public function addFriend(Person $friend) {
        $this->friends[] = $friend;
    }
    #[\ReturnTypeWillChange]
    public function bsonSerialize() {
        return array(
            "name"    => $this->name,
            "age"     => $this->age,
            "addresses" => $this->addresses,
            "friends" => $this->friends,
        );
    }
    public function bsonUnserialize(array $data): void {
        $this->name    = $data["name"];
        $this->age     = $data["age"];
        $this->addresses = $data["addresses"];
        $this->friends = $data["friends"];
    }
}
class Address implements MongoDB\BSON\Persistable {
    protected $zip;
    protected $country;

    public function __construct($zip, $country) {
        $this->zip = $zip;
        $this->country = $country;
    }
    #[\ReturnTypeWillChange]
    public function bsonSerialize() {
        return array(
            "zip"     => $this->zip,
            "country" => $this->country,
        );
    }
    public function bsonUnserialize(array $data): void {
        $this->zip = $data["zip"];
        $this->country = $data["country"];
    }
}


