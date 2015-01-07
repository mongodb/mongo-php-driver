<?php

if ( ! extension_loaded("phongo")) {
    exit("skip phongo not loaded");
}

require __DIR__ . "/" . "basic.inc";
$testdir = basename(dirname(realpath($_SERVER["SCRIPT_FILENAME"])));

switch($testdir){
case "replicaset":
case "connect":
    if (!$orch->ping() || getenv("TRAVIS") || !MONGODB_STANDALONE_AUTH_URI) {
        exit("skip these tests in $testdir require Mongo Orchestration + vagrant");
    }
    break;
}

CLEANUP();
