--TEST--
Cluster Integration Test#013: A stepped-down primary triggers a refresh
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
  'members' => 
  array (
    0 => 
    array (
      'rsParams' => 
      array (
        'priority' => 2,
      ),
      'server_id' => 'serverA',
    ),
    1 => 
    array (
      'rsParams' => 
      array (
        'priority' => 1,
      ),
      'server_id' => 'serverB',
    ),
    2 => 
    array (
      'rsParams' => 
      array (
        'arbiterOnly' => true,
      ),
      'server_id' => 'serverC',
    ),
  ),
  'name' => 'mongod',
);


$dsn = "mongodb://serverA,serverB,serverC/?heartbeatFrequency=1&readPreference=primary";
if ($INITCONFIG) {
    $dsn = mo_post("/replica_sets", $INITCONFIG);
}

$manager = new MongoDB\Driver\Manager($dsn);

mo_post("/servers/serverA", array (
  'action' => 'stepdown',
));

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
}

sleep(1);

try {
    $wc = new MongoDB\Driver\WriteConcern(1);
    $result = $manager->executeInsert("databaseName.collectionName", array (
      'x' => 1,
    ), [], $wc);
    if ($result->getInsertedCount() == 1) {
        var_dump(array("ok" => 1));
    } else {
        var_dump(array("ok" => 0, "errmsg" => "getInsertedCount => " . $result->getInsertedCount()));
    }
} catch(Exception $e) {
    var_dump(array("ok" => 0, "errmsg" => get_class($e) . ": " . $e->getMessage()));
};

foreach($KILLLIST as $id) {
    mo_delete("/replica_sets/$id");
}
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(2) {
  ["ok"]=>
  int(0)
  ["errmsg"]=>
  string(%d) "%s"
}
array(1) {
  ["ok"]=>
  int(1)
}
===DONE===