--TEST--
MongoDB\Driver\WriteResult::getUpsertedIds() with client-generated values
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
<?php skip_if_server_version('<', '3.0'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

/* Do not test Decimal128, since it is only supported by MongoDB 3.4+.
 *
 * Do not test array or Regex types, which are not permitted to be used as an
 * ID. If a regular expression is used in upsert criteria and does not match an
 * existing document, the server generates a new ObjectId. */
$tests = [
    null,
    true,
    1,
    4.125,
    'foo',
    (object) [],
    new MongoDB\BSON\Binary('foo', MongoDB\BSON\Binary::TYPE_GENERIC),
    new MongoDB\BSON\Javascript('function(){}'),
    new MongoDB\BSON\MaxKey,
    new MongoDB\BSON\MinKey,
    new MongoDB\BSON\ObjectId('586c18d86118fd6c9012dec1'),
    new MongoDB\BSON\Timestamp(1234, 5678),
    new MongoDB\BSON\UTCDateTime('1483479256924'),
];

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite;

foreach ($tests as $value) {
    $bulk->update(['_id' => $value], ['$set' => ['x' => 1]], ['upsert' => true]);
}

$result = $manager->executeBulkWrite(NS, $bulk);

var_dump($result->getUpsertedIds());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(13) {
  [0]=>
  NULL
  [1]=>
  bool(true)
  [2]=>
  int(1)
  [3]=>
  float(4.125)
  [4]=>
  string(3) "foo"
  [5]=>
  object(stdClass)#%d (%d) {
  }
  [6]=>
  object(MongoDB\BSON\Binary)#%d (%d) {
    ["data"]=>
    string(3) "foo"
    ["type"]=>
    int(0)
  }
  [7]=>
  object(MongoDB\BSON\Javascript)#%d (%d) {
    ["code"]=>
    string(12) "function(){}"
    ["scope"]=>
    NULL
  }
  [8]=>
  object(MongoDB\BSON\MaxKey)#%d (%d) {
  }
  [9]=>
  object(MongoDB\BSON\MinKey)#%d (%d) {
  }
  [10]=>
  object(MongoDB\BSON\ObjectId)#%d (%d) {
    ["oid"]=>
    string(24) "586c18d86118fd6c9012dec1"
  }
  [11]=>
  object(MongoDB\BSON\Timestamp)#%d (%d) {
    ["increment"]=>
    string(4) "1234"
    ["timestamp"]=>
    string(4) "5678"
  }
  [12]=>
  object(MongoDB\BSON\UTCDateTime)#%d (%d) {
    ["milliseconds"]=>
    string(13) "1483479256924"
  }
}
===DONE===
