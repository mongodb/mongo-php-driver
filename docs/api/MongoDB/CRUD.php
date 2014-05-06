<?php
namespace MongoDB;

interface WriteBatch {
    function add($document);
}


class InsertBatch implements WriteBatch {
    function add($document) {
        $this->documents[] = $document;
    }
}

class UpdateBatch implements WriteBatch {
    function add($document) {
        $this->documents[] = $document;
    }
}

class DeleteBatch implements WriteBatch {
    function add($document) {
        $this->documents[] = $document;
    }
}

class Query {
    function __construct($query) {}
}
class Command {
    function __construct($command) {}
}




class WriteResults {

    /* Returns nModified, n, nUserted, .. */
    function getNumbers() {
    }

    /* Returns how many operations were executed */
    function getOpCount() {
    }

    /* Returns the server that the operation was executed on */
    function getServer() {
    }

    /* Any stats available from the server */
    function getTimer() {
    }
}

class QueryResults {
    function __construct(Server $server, $cursor_id, $firstBatch) {
    }
}


