--TEST--
MongoDB\Driver\Manager::executeWriteCommand() throws CommandException for unsupported update operator
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '3.4'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

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
--EXPECTF--
MongoDB\Driver\Exception\CommandException(9): Either an update or remove=true must be specified
object(stdClass)#%d (%d) {
  ["ok"]=>
  float(0)
  ["errmsg"]=>
  string(49) "Either an update or remove=true must be specified"
  ["code"]=>
  int(9)
  ["codeName"]=>
  string(13) "FailedToParse"%A
}
===DONE===
