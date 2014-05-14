<?php

$ordered = true;

$insertBatch = new \MongoDB\Batch\InsertBatch($ordered);
$insertBatch
    ->add($hannes)
    ->add($hayley);

$w = 'majority';
$wtimeout = 500;

$writeConcern = new \MongoDB\WriteConcern($w, $wtimeout);

$mm = new \MongoDB\Manager("mongodb://server1,server2/?replicaSet=FOOBAR");

$result = $mm->executeWrite("db.collection", $insertBatch, $writeConcern);

assert($result instanceof \MongoDB\WriteResult);

printf(
    "Inserted %d documents %s in %d msec\n",
    $result->getOpCount(),
    $result->getServer()->getHostname(),
    $result->getTimer()
);
