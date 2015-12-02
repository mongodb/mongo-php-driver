--TEST--
PHPC-487: check_closed stream handler should report closed socket as closed
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; SLOW(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

function mo_delete($id) {
    $url = getMOUri() . $id;

    $opts = array("http" =>
        array(
            "method"  => "DELETE",
            "timeout" => 60,
            "header"  => "Accept: application/json\r\n" .
            "Content-type: application/x-www-form-urlencoded",
            "ignore_errors" => true,
        ),
    );

    $context = stream_context_create($opts);
    $out = file_get_contents($url, false, $context);
}

function mo_post($url, $body) {
    global $KILLLIST;

    $url = getMOUri() . $url;

    $opts = array("http" =>
        array(
            "method"  => "POST",
            "timeout" => 60,
            "header"  => "Accept: application/json\r\n" .
            "Content-type: application/x-www-form-urlencoded",
            "content" => json_encode($body),
            "ignore_errors" => true,
        ),
    );

    $context = stream_context_create($opts);
    $out = file_get_contents($url, false, $context);
    $array = json_decode($out, true);
    if ($array && !empty($array["mongodb_uri"])) {
        $KILLLIST[] = $array["id"];
        return $array["mongodb_uri"];
    }
}
$KILLLIST = array();

$dsn = mo_post("/servers", [
    'id' => 'serverA',
    'name' => 'mongod',
]);

$m = new MongoDB\Driver\Manager($dsn);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['x' => 1]);
$wr = $m->executeBulkWrite(NS, $bulk);
var_dump($wr->getInsertedCount());

mo_post("/servers/serverA", ['action' => 'stop']);

echo throws(function() use ($m) {
    $bulk = new MongoDB\Driver\BulkWrite;
    $bulk->insert(['x' => 1]);
    $wr = $m->executeBulkWrite(NS, $bulk);
    var_dump($wr->getInsertedCount());
}, 'MongoDB\Driver\Exception\RuntimeException'), "\n";

mo_post("/servers/serverA", ['action' => 'restart']);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['x' => 1]);
$wr = $m->executeBulkWrite(NS, $bulk);
var_dump($wr->getInsertedCount());

foreach($KILLLIST as $id) {
    mo_delete("/servers/$id");
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
int(1)
OK: Got MongoDB\Driver\Exception\RuntimeException
Failed to read 4 bytes from socket within 300000 milliseconds.
int(1)
===DONE===
