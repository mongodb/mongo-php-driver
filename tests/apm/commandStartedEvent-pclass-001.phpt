--TEST--
PHPC-2743: CommandStartedEvent does not infer a class from __pclass
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class Gadget implements MongoDB\BSON\Persistable
{
    public function bsonSerialize(): array
    {
        return [];
    }

    public function bsonUnserialize(array $data): void
    {
        echo "Gadget::bsonUnserialize() should not be called\n";
    }
}

class MySubscriber implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event): void
    {
        /* getCommand() and the debug handler decode the observed command
         * independently, so both are exercised here. */
        $command = $event->getCommand();

        /* The injected __pclass sits in the filter for a query and at the root
         * for a command document. Report whichever applies. Every access is
         * null-safe so that an inferred class still reports rather than
         * aborting the test. */
        $subject = $command->filter ?? $command;

        printf("subject: %s\n", get_debug_type($subject));
        printf("subject.__pclass: %s\n", get_debug_type($subject->__pclass ?? null));

        if (isset($subject->nested)) {
            printf("subject.nested: %s\n", get_debug_type($subject->nested));
            printf("subject.nested.__pclass: %s\n", get_debug_type($subject->nested->__pclass ?? null));
        }

        /* The debug handler decodes the command a second time. Assert only that
         * it reports no Gadget, rather than matching the whole format. */
        ob_start();
        var_dump($event);
        $debug = ob_get_clean();
        printf("debug has Gadget instance: %s\n", str_contains($debug, 'object(Gadget)') ? 'yes' : 'no');
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event): void
    {
    }
}

MongoDB\Driver\Monitoring\addSubscriber(new MySubscriber());

$manager = create_test_manager();
$pclass = sprintf('{"$binary":{"base64":"%s","subType":"80"}}', base64_encode('Gadget'));

echo "\n=== __pclass nested in a query filter ===\n";
$manager->executeQuery(NS, new MongoDB\Driver\Query(
    MongoDB\BSON\Document::fromJSON(sprintf('{"__pclass":%s}', $pclass))
));

echo "\n=== __pclass nested deeper in a query filter ===\n";
$manager->executeQuery(NS, new MongoDB\Driver\Query(
    MongoDB\BSON\Document::fromJSON(sprintf('{"nested":{"__pclass":%s}}', $pclass))
));

echo "\n=== __pclass at the root of a command document ===\n";
try {
    $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(
        MongoDB\BSON\Document::fromJSON(sprintf('{"ping":1,"__pclass":%s}', $pclass))
    ));
} catch (MongoDB\Driver\Exception\Exception $e) {
    /* The server may reject an unrecognized field, which is irrelevant here:
     * the event is dispatched before the command is sent. */
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
=== __pclass nested in a query filter ===
subject: stdClass
subject.__pclass: MongoDB\BSON\Binary
debug has Gadget instance: no

=== __pclass nested deeper in a query filter ===
subject: stdClass
subject.__pclass: null
subject.nested: stdClass
subject.nested.__pclass: MongoDB\BSON\Binary
debug has Gadget instance: no

=== __pclass at the root of a command document ===
subject: stdClass
subject.__pclass: MongoDB\BSON\Binary
debug has Gadget instance: no
===DONE===
