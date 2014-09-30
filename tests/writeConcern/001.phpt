--TEST--
MongoDB\WriteConcern#001
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php 
require_once "tests/utils/basic.inc";

$mc = new MongoDB\Manager(MONGODB_URI);


$w = new MongoDB\WriteConcern(MongoDB\WriteConcern::MAJORITY);
$w2 = new MongoDB\WriteConcern(MongoDB\WriteConcern::MAJORITY, 1000);

$w3 = new MongoDB\WriteConcern(2);
$w4 = new MongoDB\WriteConcern(2, 2000);

$w5 = new MongoDB\WriteConcern("tagname");
$w6 = new MongoDB\WriteConcern("string", 3000);

$w7 = new MongoDB\WriteConcern("string", 3000, false, false);
$w8 = new MongoDB\WriteConcern("string", 3000, true, true);

$w9 = new MongoDB\WriteConcern("string", 3000, true, false);
$w10= new MongoDB\WriteConcern("string", 3000, false, true);



try {
    new MongoDB\WriteConcern("string", 3000, false, true, 1);
} catch(InvalidArgumentException $e) {
    echo $e->getMessage(), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
MongoDB\WriteConcern::__construct() expects at most 4 parameters, 5 given
===DONE===
