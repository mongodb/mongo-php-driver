--TEST--
Sort query option is always serialized as a BSON document
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

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
object(MongoDB\Driver\Query)#%d (%d) {
  ["filter"]=>
  object(stdClass)#%d (%d) {
  }
  ["options"]=>
  object(stdClass)#%d (%d) {
    ["sort"]=>
    object(stdClass)#%d (%d) {
      [%r(0|"0")%r]=>
      int(1)
    }
  }
  ["readConcern"]=>
  NULL
}
0
1
2
3
4
===DONE===
