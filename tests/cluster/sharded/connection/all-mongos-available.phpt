--TEST--
Cluster Integration Test#024: Successful read with all mongos' available
--SKIPIF--
<?php require strstr(__DIR__, "tests/", true) . "tests/utils/basic-skipif.inc"; SLOW(); ?>
--FILE--
<?php
require_once strstr(__DIR__, "tests/", true) . "tests/utils/basic.inc";
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

$INITCONFIG = array (
  'id' => 'integration_tests',
  'routers' => 
  array (
    0 => 
    array (
      'server_id' => 'mongosA',
    ),
    1 => 
    array (
      'server_id' => 'mongosB',
    ),
  ),
  'shards' => 
  array (
    0 => 
    array (
      'id' => 'sh1',
    ),
  ),
  'name' => 'mongod',
);


$dsn = "mongodb://mongosA,mongosB";
if ($INITCONFIG) {
    $dsn = mo_post("/sharded_clusters", $INITCONFIG);
}

$manager = new MongoDB\Driver\Manager($dsn);

try {
    $wc = new MongoDB\Driver\WriteConcern(1);
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(array (
      'x' => 1,
    ));
    $result = $manager->executeBulkWrite("databaseName.collectionName", $bulk, $wc);
    if ($result->getInsertedCount() == 1) {
        var_dump(array("ok" => 1));
    } else {
        var_dump(array("ok" => 0, "errmsg" => "getInsertedCount => " . $result->getInsertedCount()));
    }
} catch(Exception $e) {
    var_dump(array("ok" => 0, "errmsg" => get_class($e) . ": " . $e->getMessage()));
}

try {
    $query = new MongoDB\Driver\Query(array());
    $result = $manager->executeQuery("databaseName.collectionName", $query)->toArray();
    if ($result) {
        var_dump(array("ok" => 1));
    } else {
        var_dump(array("ok" => 0, "errmsg" => "Empty result"));
    }
} catch(Exception $e) {
    var_dump(array("ok" => 0, "errmsg" => get_class($e) . ": " . $e->getMessage()));
};

foreach($KILLLIST as $id) {
    mo_delete("/sharded_clusters/$id");
}
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(1) {
  ["ok"]=>
  int(1)
}
array(1) {
  ["ok"]=>
  int(1)
}
===DONE===