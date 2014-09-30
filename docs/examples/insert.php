<?php
include "model.php";

$ordered = true;

$insertBatch = new \MongoDB\WriteBatch($ordered);
$hannes_id = $insertBatch->insert($hannes);
$hayley_id = $insertBatch->insert($hayley);

$mm = new \MongoDB\Manager("mongodb://localhost:27017");

$w = 1;
$wtimeout = 1000;
$writeConcern = new \MongoDB\WriteConcern($w, $wtimeout);
$result = $mm->executeWriteBatch("db.collection", $insertBatch, $writeConcern);

assert($result instanceof \MongoDB\WriteResult);

printf(
    "Inserted %d documents to %s\n",
    $result->getNumInserted(),
    $result->getServer()->getHost()
);
printf("hannes: %s (%s)\nhayley: %s (%s)\n", $hannes_id, get_class($hannes_id), $hayley_id, get_class($hannes_id));

