<?php

$batch = new \MongoDB\Batch\InsertBatch();
$batch
    ->add($hannes)
    ->add($hayley);

$writeOptions = array(
    "ordered" => true,
    "writeConcern" => array(
        "w" => 3,
        "wtimeout" => 42,
    ),
);

$mm = new \MongoDB\Manager("mongodb://server1,server2/?replicaSet=FOOBAR");

$results = $mm->executeWrite("db.collection", $batch, $writeOptions);

assert($results instanceof \MongoDB\WriteResults);

printf(
    "Inserted %d documents %s in %d msec\n",
    $results->getOpCount(),
    $results->getServer()->getHostname(),
    $results->getTimer()
);
