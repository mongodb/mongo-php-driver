--TEST--
Cluster Integration Test#017: Read failure when standlone is not available
--SKIPIF--
<?php require strstr(__DIR__, "tests/", true) . "tests/utils/basic-skipif.inc"; ?>
--FILE--
<?php
require_once strstr(__DIR__, "tests/", true) . "tests/utils/basic.inc";
function mo_delete($id) {
    $url = getMOUri() . $id;

    $opts = array("http" =>
        array(
            "method"  => "DELETE",
            "timeout" => 30,
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
            "timeout" => 30,
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
  'id' => 'serverA',
  'name' => 'mongod',
);


$dsn = "mongodb://serverA";
if ($INITCONFIG) {
    $dsn = mo_post("/servers", $INITCONFIG);
}

$manager = new MongoDB\Driver\Manager($dsn);

try {
    $wc = new MongoDB\Driver\WriteConcern(1);
    $result = $manager->executeInsert("databaseName.collectionName", array (
      'x' => 1,
    ), $wc);
    if ($result->getInsertedCount() == 1) {
        var_dump(array("ok" => 1));
    } else {
        var_dump(array("ok" => 0, "errmsg" => "getInsertedCount => " . $result->getInsertedCount()));
    }
} catch(Exception $e) {
    var_dump(array("ok" => 0, "errmsg" => get_class($e) . ": " . $e->getMessage()));
}

mo_post("/servers/serverA", array (
  'action' => 'stop',
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
};

foreach($KILLLIST as $id) {
    mo_delete("/servers/$id");
}
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(1) {
  ["ok"]=>
  int(1)
}
array(2) {
  ["ok"]=>
  int(0)
  ["errmsg"]=>
  string(%d) "%s"
}
===DONE===