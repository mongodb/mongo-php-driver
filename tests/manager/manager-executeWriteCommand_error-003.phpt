--TEST--
MongoDB\Driver\Manager::executeWriteCommand() throws CommandException for unsupported update operator
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('STANDALONE'); NEEDS_ATLEAST_MONGODB_VERSION(STANDALONE, "3.4"); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$command = new MongoDB\Driver\Command([
    'findAndModify' => COLLECTION_NAME,
    'query' => ['_id' => 'foo'],
    'upsert' => true,
    'new' => true,
]);

try {
    $manager->executeWriteCommand(DATABASE_NAME, $command);
} catch (MongoDB\Driver\Exception\CommandException $e) {
    printf("%s(%d): %s\n", get_class($e), $e->getCode(), $e->getMessage());
    var_dump($e->getResultDocument());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
MongoDB\Driver\Exception\CommandException(9): Either an update or remove=true must be specified
object(stdClass)#4 (4) {
  ["ok"]=>
  float(0)
  ["errmsg"]=>
  string(49) "Either an update or remove=true must be specified"
  ["code"]=>
  int(9)
  ["codeName"]=>
  string(13) "FailedToParse"
}
===DONE===
