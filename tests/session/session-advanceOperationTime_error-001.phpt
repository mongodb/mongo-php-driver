--TEST--
MongoDB\Driver\Session::advanceOperationTime() with TimestampInterface exceptions
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_replica_set_or_sharded_cluster_with_replica_set(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MyTimestamp implements MongoDB\BSON\TimestampInterface
{
    private $failIncrement;
    private $failTimestamp;

    public function __construct($failIncrement, $failTimestamp)
    {
        $this->failIncrement = $failIncrement;
        $this->failTimestamp = $failTimestamp;
    }

    public function getIncrement(): int
    {
        if ($this->failIncrement) {
            throw new Exception('getIncrement() failed');
        }

        return 5678;
    }

    public function getTimestamp(): int
    {
        if ($this->failTimestamp) {
            throw new Exception('getTimestamp() failed');
        }

        return 1234;
    }

    public function __toString(): string
    {
        return sprintf('[%d:%d]', $this->getIncrement(), $this->getTimestamp());
    }
}

$manager = create_test_manager();
$session = $manager->startSession();

echo "Initial operation time of session:\n";
var_dump($session->getOperationTime());

$timestamps = [
    new MyTimestamp(true, false),
    new MyTimestamp(false, true),
    new MyTimestamp(true, true),
];

foreach ($timestamps as $timestamp) {
    echo "\n", throws(function() use ($session, $timestamp) {
        $session->advanceOperationTime($timestamp);
    }, 'Exception'), "\n";

    echo "\nOperation time after advancing session fails:\n";
    var_dump($session->getOperationTime());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Initial operation time of session:
NULL

OK: Got Exception
getIncrement() failed

Operation time after advancing session fails:
NULL

OK: Got Exception
getTimestamp() failed

Operation time after advancing session fails:
NULL

OK: Got Exception
getTimestamp() failed

Operation time after advancing session fails:
NULL
===DONE===
