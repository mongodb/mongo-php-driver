--TEST--
MongoDB\Driver\Session::advanceOperationTime() with TimestampInterface exceptions
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_replica_set_or_mongos_with_replica_set(); ?>
<?php skip_if_server_version('<', '3.6'); ?>
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

    public function getIncrement()
    {
        if ($this->failIncrement) {
            throw new Exception('getIncrement() failed');
        }

        return 5678;
    }

    public function getTimestamp()
    {
        if ($this->failTimestamp) {
            throw new Exception('getTimestamp() failed');
        }

        return 1234;
    }

    public function __toString()
    {
        return sprintf('[%d:%d]', $this->getIncrement(), $this->getTimestamp());
    }
}

$manager = new MongoDB\Driver\Manager(URI);
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
