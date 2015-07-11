--TEST--
Sort query option is always serialized as a BSON document
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php CLEANUP(STANDALONE); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$bulkWrite = new MongoDB\Driver\BulkWrite;

for ($i = 0; $i < 5; $i++) {
    $bulkWrite->insert(array('_id' => $i, '0' => 4 - $i));
}

$writeResult = $manager->executeBulkWrite(NS, $bulkWrite);
printf("Inserted: %d\n", $writeResult->getInsertedCount());

$query = new MongoDB\Driver\Query(array(), array(
    'sort' => array('0' => 1),
));

var_dump($query);

$cursor = $manager->executeQuery(NS, $query);

/* Numeric keys of stdClass instances cannot be directly accessed, so ensure the
 * document is decoded as a PHP array.
 */
$cursor->setTypeMap(array('root' => 'array'));

foreach ($cursor as $document) {
    echo $document['0'] . "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Inserted: 5
object(MongoDB\Driver\Query)#%d (6) {
  ["query"]=>
  object(stdClass)#%d (2) {
    ["$orderby"]=>
    object(stdClass)#%d (1) {
      [0]=>
      int(1)
    }
    ["$query"]=>
    object(stdClass)#%d (0) {
    }
  }
  ["selector"]=>
  NULL
  ["flags"]=>
  int(0)
  ["skip"]=>
  int(0)
  ["limit"]=>
  int(0)
  ["batch_size"]=>
  int(0)
}
0
1
2
3
4
===DONE===
