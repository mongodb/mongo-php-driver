--TEST--
MongoDB\Driver\Session::startTransaction() Committing a transaction with example for how to handle failures
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_clean(DATABASE_NAME, COLLECTION_NAME . '_employees'); ?>
<?php skip_if_not_clean(DATABASE_NAME, COLLECTION_NAME . '_events'); ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_server_version('<', '4.0'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$EMPLOYEES_COL = COLLECTION_NAME . '_employees';
$EVENTS_COL =    COLLECTION_NAME . '_events';

$manager = new MongoDB\Driver\Manager(URI);

/* Create collections as that can't be (automatically) done in a transaction */
$cmd = new \MongoDB\Driver\Command([
    'create' => $EMPLOYEES_COL
]);
$manager->executeCommand(DATABASE_NAME, $cmd);

$cmd = new \MongoDB\Driver\Command([
    'create' => $EVENTS_COL
]);
$manager->executeCommand(DATABASE_NAME, $cmd);


/* Do the transaction */
$session = $manager->startSession();

$session->startTransaction( [
    'readConcern' => new \MongoDB\Driver\ReadConcern( "snapshot" ),
    'writeConcern' => new \MongoDB\Driver\WriteConcern( \MongoDB\Driver\WriteConcern::MAJORITY )
] );

while (true) {
    try {
        $cmd = new \MongoDB\Driver\Command( [
            'update' => $EMPLOYEES_COL,
            'updates' => [
                [
                    'q' => [ 'employee' => 3 ],
                    'u' => [ '$set' => [ 'status' => 'Inactive' ] ],
                ]
            ]
        ] );
        $manager->executeCommand(DATABASE_NAME, $cmd, ['session' => $session]);
 
        $cmd = new \MongoDB\Driver\Command( [
            'insert' => $EVENTS_COL,
            'documents' => [
                [ 'employee' => 3, 'status' => [ 'new' => 'Inactive', 'old' => 'Active' ] ]
            ]
        ] );
        $manager->executeCommand(DATABASE_NAME, $cmd, ['session' => $session]);

        $session->commitTransaction();
        echo "Transaction committed.\n";break;
    } catch (\MongoDB\Driver\Exception\Exception $e) {
        $rd = $e->getResultDocument();

        if (isset($rd->errorLabels) && in_array('TransientTransactionError', $rd->errorLabels)) {
            echo "Temporary error: ", $e->getMessage(), ", retrying...\n";
            $rd = $e->getResultDocument();
            var_dump($rd);
            continue;
        } else {
            var_dump($e);
        }
        break;
    }
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Transaction committed.
===DONE===
