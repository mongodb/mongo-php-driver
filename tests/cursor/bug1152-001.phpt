--TEST--
PHPC-1152: Command cursors should use the same session for getMore and killCursors (implicit)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '3.6'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class Test implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    private $lsidByCursorId = [];
    private $lsidByRequestId = [];

    public function executeCommand()
    {
        $manager = new MongoDB\Driver\Manager(URI);

        $bulk = new MongoDB\Driver\BulkWrite;
        $bulk->insert(['_id' => 1]);
        $bulk->insert(['_id' => 2]);
        $bulk->insert(['_id' => 3]);
        $manager->executeBulkWrite(NS, $bulk);

        $command = new MongoDB\Driver\Command([
            'aggregate' => COLLECTION_NAME,
            'pipeline' => [['$match' => new stdClass]],
            'cursor' => ['batchSize' => 2],
        ]);

        MongoDB\Driver\Monitoring\addSubscriber($this);

        /* By creating two cursors with the same name, PHP's reference counting
         * will destroy the first after the second is created. Note that
         * mongoc_cursor_destroy also destroys implicit sessions and returns
         * them to the LIFO pool. This sequencing allows us to test that getMore
         * and killCursors use the session ID corresponding to the original
         * aggregate command. */
        $cursor = $manager->executeCommand(DATABASE_NAME, $command);
        $cursor->toArray();

        $cursor = $manager->executeCommand(DATABASE_NAME, $command);
        $cursor->toArray();

        $cursor = $manager->executeCommand(DATABASE_NAME, $command);
        $cursor = $manager->executeCommand(DATABASE_NAME, $command);
        unset($cursor);

        MongoDB\Driver\Monitoring\removeSubscriber($this);

        /* We should expect two unique session IDs over the course of the test,
         * since at most two implicit sessions would have been in use at any
         * given time. */
        printf("Unique session IDs used: %d\n", count(array_unique($this->lsidByRequestId)));
    }

    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event)
    {
        $requestId = $event->getRequestId();
        $sessionId = bin2hex((string) $event->getCommand()->lsid->id);

        printf("%s session ID: %s\n", $event->getCommandName(), $sessionId);

        if ($event->getCommandName() === 'aggregate') {
            if (isset($this->lsidByRequestId[$requestId])) {
                throw new UnexpectedValueException('Previous command observed for request ID: ' . $requestId);
            }

            $this->lsidByRequestId[$requestId] = $sessionId;
        }

        if ($event->getCommandName() === 'getMore') {
            $cursorId = (string) $event->getCommand()->getMore;

            if ( ! isset($this->lsidByCursorId[$cursorId])) {
                throw new UnexpectedValueException('No previous command observed for cursor ID: ' . $cursorId);
            }

            printf("getMore used same session as aggregate: %s\n", $sessionId === $this->lsidByCursorId[$cursorId] ? 'yes' : 'no');
        }

        if ($event->getCommandName() === 'killCursors') {
            $cursorId = (string) $event->getCommand()->cursors[0];

            if ( ! isset($this->lsidByCursorId[$cursorId])) {
                throw new UnexpectedValueException('No previous command observed for cursor ID: ' . $cursorId);
            }

            printf("killCursors used same session as aggregate: %s\n", $sessionId === $this->lsidByCursorId[$cursorId] ? 'yes' : 'no');
        }
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event)
    {
        /* Associate the aggregate's session ID with its cursor ID so it can be
         * looked up by the subsequent getMore or killCursors */
        if ($event->getCommandName() === 'aggregate') {
            $cursorId = (string) $event->getReply()->cursor->id;
            $requestId = $event->getRequestId();

            $this->lsidByCursorId[$cursorId] = $this->lsidByRequestId[$requestId];
        }
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event)
    {
    }
}

(new Test)->executeCommand();

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
aggregate session ID: %x
getMore session ID: %x
getMore used same session as aggregate: yes
aggregate session ID: %x
getMore session ID: %x
getMore used same session as aggregate: yes
aggregate session ID: %x
aggregate session ID: %x
killCursors session ID: %x
killCursors used same session as aggregate: yes
killCursors session ID: %x
killCursors used same session as aggregate: yes
Unique session IDs used: 2
===DONE===
