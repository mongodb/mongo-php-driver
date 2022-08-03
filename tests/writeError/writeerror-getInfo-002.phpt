--TEST--
MongoDB\Driver\WriteError::getInfo() exposes writeError.errInfo
--DESCRIPTION--
CRUD spec prose test #2
https://github.com/mongodb/specifications/blob/master/source/crud/tests/README.rst#writeerror-details-exposes-writeerrors-errinfo
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_server_version('<', '5.0'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class Test implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    private $errInfo;

    public function execute()
    {
        $manager = create_test_manager();

        $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command([
            'create' => COLLECTION_NAME,
            'validator' => ['x' => ['$type' => 'string']],
        ]));

        $bulk = new MongoDB\Driver\BulkWrite;
        $bulk->insert(['x' => 1]);

        MongoDB\Driver\Monitoring\addSubscriber($this);

        try {
            $manager->executeBulkWrite(NS, $bulk);
        } catch (MongoDB\Driver\Exception\BulkWriteException $e) {
            $writeError = $e->getWriteResult()->getWriteErrors()[0];

            var_dump($writeError->getCode()); // DocumentValidationFailure(121)

            /* Note: we intentionally do not assert the contents of errInfo
             * since its structure could change between server versions. */
            var_dump($writeError->getInfo() instanceof stdClass);
            var_dump($this->errInfo instanceof stdClass);
            var_dump($writeError->getInfo() == $this->errInfo);
        }

        MongoDB\Driver\Monitoring\removeSubscriber($this);
    }

    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event): void
    {
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
        if ($event->getCommandName() === 'insert') {
            $this->errInfo = $event->getReply()->writeErrors[0]->errInfo ?? null;
        }
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event): void
    {
    }
}

(new Test)->execute();

?>
===DONE===
--EXPECTF--
int(121)
bool(true)
bool(true)
bool(true)
===DONE===
