--TEST--
MongoDB\Driver\Manager::startSession() with wrong defaultTransactionOptions
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$options = [
    [ 'maxCommitTimeMS' => -1 ],
    [ 'readConcern' => 42 ],
    [ 'readConcern' => new stdClass ],
    [ 'readConcern' => new \MongoDB\Driver\WriteConcern( 2 ) ],
    [ 'readPreference' => 42 ],
    [ 'readPreference' => new stdClass ],
    [ 'readPreference' => new \MongoDB\Driver\ReadConcern( \MongoDB\Driver\ReadConcern::LOCAL ) ],
    [ 'writeConcern' => 42 ],
    [ 'writeConcern' => new stdClass ],
    [ 'writeConcern' => new \MongoDB\Driver\ReadPreference( \MongoDB\Driver\ReadPreference::RP_SECONDARY ) ],

    [
        'readConcern' => new \MongoDB\Driver\ReadConcern( \MongoDB\Driver\ReadConcern::LOCAL ),
        'readPreference' => new \MongoDB\Driver\ReadConcern( \MongoDB\Driver\ReadConcern::LOCAL ),
    ],
    [
        'readConcern' => new \MongoDB\Driver\ReadConcern( \MongoDB\Driver\ReadConcern::LOCAL ),
        'writeConcern' => new \MongoDB\Driver\ReadPreference( \MongoDB\Driver\ReadPreference::RP_SECONDARY ),
    ],
    [
        'readPreference' => new \MongoDB\Driver\ReadPreference( \MongoDB\Driver\ReadPreference::RP_SECONDARY ),
        'writeConcern' => new \MongoDB\Driver\ReadPreference( \MongoDB\Driver\ReadPreference::RP_SECONDARY ),
    ],

    42,
    new stdClass,
];

foreach ($options as $txnOptions) {
    echo throws(function() use ($manager, $txnOptions) {
        $manager->startSession([
            'defaultTransactionOptions' => $txnOptions
        ]);
    }, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";
}

echo raises(function() use ($manager) {
    $manager->startSession([
        'defaultTransactionOptions' => [ 'maxCommitTimeMS' => new stdClass ]
    ]);
}, E_NOTICE | E_WARNING), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "maxCommitTimeMS" option to be >= 0, -1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readConcern" option to be MongoDB\Driver\ReadConcern, int%S given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readConcern" option to be MongoDB\Driver\ReadConcern, stdClass given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readConcern" option to be MongoDB\Driver\ReadConcern, MongoDB\Driver\WriteConcern given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readPreference" option to be MongoDB\Driver\ReadPreference, int%S given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readPreference" option to be MongoDB\Driver\ReadPreference, stdClass given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readPreference" option to be MongoDB\Driver\ReadPreference, MongoDB\Driver\ReadConcern given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "writeConcern" option to be MongoDB\Driver\WriteConcern, int%S given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "writeConcern" option to be MongoDB\Driver\WriteConcern, stdClass given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "writeConcern" option to be MongoDB\Driver\WriteConcern, MongoDB\Driver\ReadPreference given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readPreference" option to be MongoDB\Driver\ReadPreference, MongoDB\Driver\ReadConcern given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "writeConcern" option to be MongoDB\Driver\WriteConcern, MongoDB\Driver\ReadPreference given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "writeConcern" option to be MongoDB\Driver\WriteConcern, MongoDB\Driver\ReadPreference given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "defaultTransactionOptions" option to be an array, int%S given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "defaultTransactionOptions" option to be an array, stdClass given
OK: Got %r(E_NOTICE|E_WARNING)%r
Object of class stdClass could not be converted to int
===DONE===
