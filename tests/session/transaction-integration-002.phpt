--TEST--
MongoDB\Driver\Session::startTransaction() Transient Error Test
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

/* Insert Data */
$bw = new \MongoDB\Driver\BulkWrite();
$bw->insert( [ '_id' => 0, 'msg' => 'Initial Value' ] );
$manager->executeBulkWrite(NS, $bw);

/* First 'thread', try to update document, but don't close transaction */
$sessionA = $manager->startSession();
$sessionA->startTransaction( [
    'readConcern' => new \MongoDB\Driver\ReadConcern( "snapshot" ),
    'writeConcern' => new \MongoDB\Driver\WriteConcern( \MongoDB\Driver\WriteConcern::MAJORITY )
] );

$cmd = new \MongoDB\Driver\Command( [
    'update' => COLLECTION_NAME,
    'updates' => [
        [
            'q' => [ '_id' => 0 ],
            'u' => [ '$set' => [ 'msg' => 'Update from session A' ] ],
        ]
    ]
] );
$manager->executeCommand(DATABASE_NAME, $cmd, ['session' => $sessionA]);
 

/* Second 'thread', try to update the same document, should trigger exception. In handler, commit
 * first settion, verify result, and redo this transaction. */
$sessionB = $manager->startSession();
$sessionB->startTransaction( [
    'readConcern' => new \MongoDB\Driver\ReadConcern( "snapshot" ),
    'writeConcern' => new \MongoDB\Driver\WriteConcern( \MongoDB\Driver\WriteConcern::MAJORITY )
] );

try {
    $cmd = new \MongoDB\Driver\Command( [
        'update' => COLLECTION_NAME,
        'updates' => [
            [
                'q' => [ '_id' => 0 ],
                'u' => [ '$set' => [ 'msg' => 'Update from session B' ] ],
            ]
        ]
    ] );
    $manager->executeCommand(DATABASE_NAME, $cmd, ['session' => $sessionB]);
} catch (MongoDB\Driver\Exception\CommandException $e) {
    echo $e->hasErrorLabel('TransientTransactionError') ?
        "found a TransientTransactionError" : "did NOT get a TransientTransactionError", "\n";
}
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
found a TransientTransactionError
===DONE===
