<?php

$typemap = array(
    \BSON\Types\ArrayObj::CLASS => \BSON\ArrayDynamic::CLASS,
    /* \BSON\Types\ArrayObj::CLASS => \BSON\ArrayFixed::CLASS, */
    /* \BSON\Types\ArrayObj::CLASS => "array", */
    \BSON\Types\Object::CLASS   => \BSON\Object::CLASS,
    \BSON\Types\ObjectID::CLASS => \BSON\ObjectID::CLASS,
    \BSON\Types\DateTime::CLASS => \BSON\DateTime::CLASS,
);

$mm = new MongoManager(array("bson" => $typemap));



