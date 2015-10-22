<?php

$template = <<< 'TEMPLATE'
--TEST--
%DESCRIPTION%
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

$INITCONFIG = %INITCONFIG%;


$dsn = "%DSN%";
if ($INITCONFIG) {
    $dsn = mo_post("/%URLTYPE%", $INITCONFIG);
}

$manager = new MongoDB\Driver\Manager($dsn);

%TESTPLAN%;

foreach($KILLLIST as $id) {
    mo_delete("/%URLTYPE%/$id");
}
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%OUTPUT%
===DONE===
TEMPLATE;
?><?php



array_shift($argv);
foreach($argv as $jsonfile) {
    if (!file_exists($jsonfile)) {
        exit("Cannot find $jsonfile");
    }



    $test = json_decode(file_get_contents($jsonfile), true);
    if (!$test) {
        exit("Cannot decode $jsonfile");
    }

    $path = substr($jsonfile, strpos($jsonfile, "mo-tests/")+strlen("mo-tests/"));
    $dir = dirname($path);
    $fullpath = __DIR__ . "/" . "../tests/cluster/" . $dir;
    if (!is_dir($fullpath)) {
        mkdir($fullpath, 0755, true);
    }
    $phptfile = realpath($fullpath) . "/" . pathinfo($jsonfile, PATHINFO_FILENAME). ".phpt";


    echo "Converting $jsonfile to $phptfile\n";
    try {
        $phpt = convert($test, $template);
    } catch(Exception $e) {
        echo "skipped: " . $e->getMessage() . "\n";
    }

    file_put_contents($phptfile, $phpt);
}

function convert($test, $template) {

    $TESTPLAN = array();
    $OUTPUT   = "";
    $test["phases"] = isset($test["phases"]) ? $test["phases"] : array();
    foreach($test["phases"] as $phase) {
        $TESTPLAN[] = phase($phase, $output);
        if ($output) {
            ob_start();
            var_dump($output);
            $OUTPUT .= ob_get_contents();
            ob_end_clean();
            $OUTPUT = str_replace('2) "%s"', '%d) "%s"', $OUTPUT);
        }
    }
    foreach($test["tests"] as $phase) {
        $TESTPLAN[] = phase($phase, $output);
        if ($output) {
            ob_start();
            var_dump($output);
            $OUTPUT .= ob_get_contents();
            ob_end_clean();
            $OUTPUT = str_replace('2) "%s"', '%d) "%s"', $OUTPUT);
        }
    }

    $replacements = array(
        "%DSN%"         => clientSetup($test),
        "%DESCRIPTION%" => description($test),
        "%INITCONFIG%"  => initConfig($test),
        "%URLTYPE%"     => $test["type"],
        "%TESTPLAN%"    => join("\n\n", $TESTPLAN),
        "%OUTPUT%"      => rtrim($OUTPUT),
    );
    return makeTest($replacements, $template);
}

function phase($phase, &$output) {
    $output = null;

    if (isset($phase["MOOperation"])) {
        return MOOperation($phase["MOOperation"], $output);
    }
    if (isset($phase["clientHosts"])) {
        return clientHosts($phase["clientHosts"], $output);
    }
    if (isset($phase["clientOperation"])) {
        return clientOperation($phase["clientOperation"], $output);
    }
    if (isset($phase["wait"])) {
        return 'sleep(' . $phase["wait"] . ');';
    }
    throw new UnexpectedValueException("Don't know how to deal with " . json_encode($phase));
}

function MOOperation($phase, &$output) {
    $method = strtolower($phase["method"]);
    switch($method) {
    case "post":
        return 'mo_post("' . $phase["uri"] . '", ' . var_export($phase["payload"], true) . ');';

    case "delete":
        return 'mo_delete("' . $phase["uri"] . '");';

    default:
        throw new UnexpectedValueException("Don't know the method $method");
    }
}

function clientHosts($hosts, &$output) {
    $output = array();

    $retval = <<< CODE
\$clientHosts = array();

CODE;

    if (!empty($hosts['primary'])) {
        $primary = var_export($hosts['primary'], true);

        $retval .= <<< CODE
\$found = array_filter(\$manager->getServers(), function(\$server) {
    return \$server->getHost() == $primary && \$server->getType() == MongoDB\\Driver\\SERVERTYPE_RS_PRIMARY;
});
if (count(\$found) == 1) {
    \$clientHosts['primary'] = $primary;
}

CODE;
        $output['primary'] = $hosts['primary'];
    }

    if (!empty($hosts['secondaries'])) {
        foreach ($hosts['secondaries'] as $secondaryHost) {
            $secondary = var_export($secondaryHost, true);

            $retval .= <<< CODE

\$found = array_filter(\$manager->getServers(), function(\$server) {
    return \$server->getHost() == $secondary && \$server->getType() == MongoDB\\Driver\\SERVERTYPE_RS_SECONDARY;
});
if (count(\$found) == 1) {
    \$clientHosts['secondaries'][] = $secondary;
}

CODE;
            $output['secondaries'][] = $secondaryHost;
        }
    }

    $retval .= <<< CODE
var_dump(\$clientHosts);
CODE;

    return $retval;
}

function clientOperation($phase, &$output) {
    switch($phase["operation"]) {
    case "insertOne":
        $output = !isset($phase["outcome"]) ? array("ok" => 1) : $phase["outcome"];
        if (!$output["ok"]) {
            $output["errmsg"] = "%s";
        }


        $wc = !isset($phase["writeConcern"]) ? 1 : $phase["writeConcern"]["w"];

        $doc = var_export($phase["doc"], true);;
        $doc = str_replace("\n", "\n    ", $doc);

        $retval = <<< CODE
try {
    \$wc = new MongoDB\\Driver\\WriteConcern($wc);
    \$bulk = new MongoDB\Driver\BulkWrite();
    \$bulk->insert($doc);
    \$result = \$manager->executeBulkWrite("databaseName.collectionName", \$bulk, \$wc);
    if (\$result->getInsertedCount() == 1) {
        var_dump(array("ok" => 1));
    } else {
        var_dump(array("ok" => 0, "errmsg" => "getInsertedCount => " . \$result->getInsertedCount()));
    }
} catch(Exception \$e) {
    var_dump(array("ok" => 0, "errmsg" => get_class(\$e) . ": " . \$e->getMessage()));
}
CODE;
        return $retval;
        break;

    case "find":
        $output = $phase["outcome"];
        if (!$output["ok"]) {
            $output["errmsg"] = "%s";
        }
        $retval = <<< CODE
try {
    \$query = new MongoDB\\Driver\\Query(array());
    \$result = \$manager->executeQuery("databaseName.collectionName", \$query)->toArray();
    var_dump(array("ok" => 1));
} catch(Exception \$e) {
    var_dump(array("ok" => 0, "errmsg" => get_class(\$e) . ": " . \$e->getMessage()));
}
CODE;
        return $retval;
        break;
    default:
        throw new UnexpectedValueException("IDK what to do about {$phase["operation"]}");
    }
}


function clientSetUp($test) {
    $setup = $test["clientSetUp"];
    if (!$setup) {
        throw new UnexpectedValueException("No 'clientSetUp' key provided");
    }

    $seedlist = join(",", $setup["hosts"]);
    $options = array();
    if (!empty($setup["options"])) {
        $entries = array();
        foreach($setup["options"] as $k => $v) {
            switch($k) {
            case "readPreference":
                $entries[] = "$k={$v["mode"]}"; 
                break;

            case "heartbeatFrequency":
                $entries[] = "$k=$v";
                break;

            default:
                throw new Exception("Don't know how to handle '$k'");
            }
        }
        $options = join("&", $entries);
    }

    if ($options) {
        return sprintf("mongodb://%s/?%s", $seedlist, $options);
    } else {
        return sprintf("mongodb://%s", $seedlist);
    }
}

function makeTest($replacements, $template) {
    return str_replace(array_keys($replacements), array_values($replacements), $template);
}
function description($test) {
    static $count = 0;

    $desc = $test["description"];

    return sprintf("Cluster Integration Test#%03d: %s", ++$count, $desc);
}

function initConfig($test) {
    // The initConfig entry alone isn't valid MO post
    $config = $test["initConfig"];
    $config["name"] = "mongod";
    return var_export($config, true);
}


