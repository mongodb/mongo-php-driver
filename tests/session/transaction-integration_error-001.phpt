--TEST--
MongoDB\Driver\Session: Setting per-op readConcern or writeConcern in transaction (executeCommand)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_no_transactions(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

/* Create collections as that can't be (automatically) done in a transaction */
$manager->executeCommand(
    DATABASE_NAME,
    new \MongoDB\Driver\Command([ 'create' => COLLECTION_NAME ]),
    [ 'writeConcern' => new \MongoDB\Driver\WriteConcern( \MongoDB\Driver\WriteConcern::MAJORITY ) ]
);


/* Do the transaction */
$session = $manager->startSession();

$session->startTransaction( [
    'readConcern' => new \MongoDB\Driver\ReadConcern( "snapshot" ),
    'writeConcern' => new \MongoDB\Driver\WriteConcern( \MongoDB\Driver\WriteConcern::MAJORITY )
] );

echo throws(function() use ($manager, $session) {
    $cmd = new \MongoDB\Driver\Command( [
        'update' => COLLECTION_NAME,
        'updates' => [ [ 'q' => [ 'employee' => 3 ], 'u' => [ '$set' => [ 'status' => 'Inactive' ] ] ] ]
    ] );
    $manager->executeCommand(
        DATABASE_NAME,
        $cmd,
        [
            'session' => $session,
            'readConcern' => new \MongoDB\Driver\ReadConcern( \MongoDB\Driver\ReadConcern::LOCAL )
        ]
    );
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() use ($manager, $session) {
    $cmd = new \MongoDB\Driver\Command( [
        'update' => COLLECTION_NAME,
        'updates' => [ [ 'q' => [ 'employee' => 3 ], 'u' => [ '$set' => [ 'status' => 'Inactive' ] ] ] ]
    ] );
    $manager->executeCommand(
        DATABASE_NAME,
        $cmd,
        [
            'session' => $session,
            'writeConcern' => new \MongoDB\Driver\WriteConcern( \MongoDB\Driver\WriteConcern::MAJORITY )
        ]
    );
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot set read concern after starting transaction
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot set write concern after starting transaction
===DONE===
