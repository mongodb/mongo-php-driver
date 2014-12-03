<?php

$manager = new MongoDB\Manager("mongodb://localhost:27017");


/* List all available commands */

$listcommands = new MongoDB\Command(array("listCommands" => 1));
$retval = $manager->executeCommand("admin", $listcommands);
$commands = $retval->getResponseDocument();

foreach($commands["commands"] as $command => $info) {
    echo $command, "\n -- ", trim(explode("\n", $info->help)[0]), "\n";
}


echo "\n\n\n";


/* List all Databases */
$listdbs = new MongoDB\Command(array("listDatabases" => 1));
$retval = $manager->executeCommand("admin", $listdbs);
$dbs = $retval->getResponseDocument();

foreach($dbs["databases"] as $dbinfo) {
    /* Print the Database name and its size on disk */
    echo $dbinfo->name, " (", $dbinfo->sizeOnDisk / 1024 / 1024, "mb)\n";

    /* List all the Collections within that database */
    $listcollections = new MongoDB\Command(array("listCollections" => 1));
    $retval = $manager->executeCommand($dbinfo->name, $listcollections);
    $collections = $retval->getResponseDocument();

    foreach($collections["collections"] as $collinfo) {
        /* Retreieve the collection statistics for this collection */
        $collstats = new MongoDB\Command(array("collStats" => $collinfo->name));
        $retval = $manager->executeCommand($dbinfo->name, $collstats);
        $stats = $retval->getResponseDocument();

        /* Print the collection name and document count */
        echo "  - ", $collinfo->name, " (" , $stats["count"], " document(s))\n";

        /* Fetch all documents in this collection, but only retrieve the _id field */
        $query = new MongoDB\Query(array(), array("projection" => array("_id" => 1)));
        $qi = $manager->executeQuery($dbinfo->name . "." . $collinfo->name, $query);
        foreach($qi as $document) {
            /* Not all documents have _id field (special internal collections for example) */
            if (isset($document["_id"])) {
                echo "    - _id: ", $document["_id"], "\n";
            } else {
                echo "    - (noid)\n";
            }
        }
    }
}
/* The total size of all the databases */
echo "--\nTotal Size: ", $dbs["totalSize"] / 1024 / 1024, "mb\n";



