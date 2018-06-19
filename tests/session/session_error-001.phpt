--TEST--
MongoDB\Driver\Session with wrong defaultTransactionOptions
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$options = [
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
        $session = $manager->startSession([
            'defaultTransactionOptions' => $txnOptions
        ]);
    }, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readConcern" option to be MongoDB\Driver\ReadConcern, integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readConcern" option to be MongoDB\Driver\ReadConcern, stdClass given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readConcern" option to be MongoDB\Driver\ReadConcern, MongoDB\Driver\WriteConcern given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readPreference" option to be MongoDB\Driver\ReadPreference, integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readPreference" option to be MongoDB\Driver\ReadPreference, stdClass given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readPreference" option to be MongoDB\Driver\ReadPreference, MongoDB\Driver\ReadConcern given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "writeConcern" option to be MongoDB\Driver\WriteConcern, integer given
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
Expected "defaultTransactionOptions" option to be an array, integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "defaultTransactionOptions" option to be an array, stdClass given
===DONE===
