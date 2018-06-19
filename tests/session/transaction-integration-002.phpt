--TEST--
MongoDB\Driver\Session::startTransaction() Transient Error Test
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_clean(DATABASE_NAME, COLLECTION_NAME); ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_server_version('<', '4.0'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

/* Create collections as that can't be (automatically) done in a transaction */
$cmd = new \MongoDB\Driver\Command([
    'create' => COLLECTION_NAME,
]);
$manager->executeCommand(DATABASE_NAME, $cmd);

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
    $rd = $e->getResultDocument();

    echo (isset($rd->errorLabels) && in_array("TransientTransactionError", $rd->errorLabels)) ?
        "found a TransientTransactionError" : "did NOT get a TransientTransactionError", "\n";
}
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
found a TransientTransactionError
===DONE===
