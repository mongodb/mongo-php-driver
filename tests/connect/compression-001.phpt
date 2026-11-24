--TEST--
MongoDB\Driver\Manager: Connecting with compression
--SKIPIF--
<?php require __DIR__ . '/../utils/basic-skipif.inc'; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_load_balanced(); ?>
<?php

foreach (['zlib', 'zstd'] as $compressor) {
    if (get_module_info("libmongoc compression {$compressor}") !== 'enabled') {
        exit("skip libmongoc {$compressor} compression is not enabled");
    }
}

$command = new MongoDB\Driver\Command(['hello' => 1, 'compression' => ['zlib', 'zstd']]);

try {
    $reply = current(get_primary_server(URI)->executeCommand('admin', $command)->toArray());
} catch (MongoDB\Driver\Exception\CommandException $e) {
    $command = new MongoDB\Driver\Command(['isMaster' => 1, 'compression' => ['zlib', 'zstd']]);
    $reply = current(get_primary_server(URI)->executeCommand('admin', $command)->toArray());
}

$serverCompressors = isset($reply->compression) ? (array) $reply->compression : [];

foreach (['zlib', 'zstd'] as $compressor) {
    if (!in_array($compressor, $serverCompressors, true)) {
        exit("skip server does not support {$compressor} compression");
    }
}

?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

class CompressionObserver implements MongoDB\Driver\Monitoring\SDAMSubscriber
{
    private $compressors = [];

    public function serverChanged(MongoDB\Driver\Monitoring\ServerChangedEvent $event): void {}

    public function serverClosed(MongoDB\Driver\Monitoring\ServerClosedEvent $event): void {}

    public function serverHeartbeatFailed(MongoDB\Driver\Monitoring\ServerHeartbeatFailedEvent $event): void {}

    public function serverHeartbeatStarted(MongoDB\Driver\Monitoring\ServerHeartbeatStartedEvent $event): void {}

    public function serverHeartbeatSucceeded(MongoDB\Driver\Monitoring\ServerHeartbeatSucceededEvent $event): void
    {
        $reply = $event->getReply();
        if (isset($reply->compression)) {
            $this->compressors = (array) $reply->compression;
        }
    }

    public function serverOpening(MongoDB\Driver\Monitoring\ServerOpeningEvent $event): void {}

    public function topologyChanged(MongoDB\Driver\Monitoring\TopologyChangedEvent $event): void {}

    public function topologyClosed(MongoDB\Driver\Monitoring\TopologyClosedEvent $event): void {}

    public function topologyOpening(MongoDB\Driver\Monitoring\TopologyOpeningEvent $event): void {}

    public function getCompressors(): string
    {
        return implode(',', $this->compressors);
    }
}

function connect_with_compressors($compressors): string
{
    $observer = new CompressionObserver();

    $manager = create_test_manager(null, ['compressors' => $compressors], ['disableClientPersistence' => true]);
    $manager->addSubscriber($observer);

    $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));

    return $observer->getCompressors();
}

printf("String form: %s\n", connect_with_compressors('zlib,zstd'));
printf("Array form: %s\n", connect_with_compressors(['zlib', 'zstd']));

printf("Empty array: %s\n", connect_with_compressors([]) ?: '(none)');
printf("Empty string: %s\n", connect_with_compressors('') ?: '(none)');

printf("Unsupported string: %s\n", connect_with_compressors('zlib,foo'));
printf("Unsupported array element: %s\n", connect_with_compressors(['zlib', 'foo']));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
String form: zlib,zstd
Array form: zlib,zstd
Empty array: (none)
Empty string: (none)
Unsupported string: zlib
Unsupported array element: zlib
===DONE===
