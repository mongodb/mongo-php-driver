<?php

$queryDocument = array(
    '$query'   => array("name" => "Hannes"),
    '$orderBy' => array('name' => 1),
    '$comment' => 'More special stuff',
);

$queryFields = array(
    'fieldname' => 1,
    'fieldname.two' => 1,
);

$flags = EXHAUST | NO_CURSOR_TIMEOUT;
$limit = -3; // Kill server cursor after 3 documents
$skip = 0;   // Don't skip any documents

$query = new \MongoDB\Query($queryDocument, $queryFields, $flags, $limit, $skip);

$readPreference = array(
    "type" => 'secondaryPreferred',
    "tags" => array(
        array("dc:eu", "cpu:fast"),
        array("dc:eu", "cc:is"),
        array(""),
    ),
);

$mm = new \MongoDB\Manager("mongodb://server1,server2/?replicaSet=FOOBAR");

$cursor = $mm->executeQuery("db.collection", $query, $readPreference);

assert($cursor instanceof \MongoDB\Cursor);

foreach($cursor as $person) {
    printf(
        "%s has lived in %s\n",
        $person->getName(),
        $person->getAddresses()[0]->getCountry()->getName()
    );
}
