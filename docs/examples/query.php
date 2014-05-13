<?php

$criteria = array("name" => "Hannes");
$selector = array('fieldname' => 1, 'fieldname.two' => 1);

$query = new \MongoDB\Query($criteria, $selector);
$query
    ->setOrderBy(array('name' => 1))
    ->setComment('More special stuff')
    ->setLimit(-3) // Kill server cursor after 3 documents
    ->setSkip(0)   // Don't skip any documents
    ->setFlags(EXHAUST | NO_CURSOR_TIMEOUT);

$tags = array(
    array("dc:eu", "cpu:fast"),
    array("dc:eu", "cc:is"),
    array(""),
);

$readPreference = new \MongoDB\ReadPreference('secondaryPreferred', $tags);

$mm = new \MongoDB\Manager("mongodb://server1,server2/?replicaSet=FOOBAR");

$result = $mm->executeQuery("db.collection", $query, $readPreference);

assert($result instanceof \MongoDB\QueryResult);
assert($result instanceof \IteratorAggregate);
assert($result->getIterator() instanceof \MongoDB\Cursor);

foreach($result as $person) {
    printf(
        "%s has lived in %s\n",
        $person->getName(),
        $person->getAddresses()[0]->getCountry()->getName()
    );
}
