<?php

if ( ! extension_loaded("phongo")) {
    exit("skip phongo not loaded");
}

require __DIR__ . "/" . "basic.inc";
$testdir = basename(dirname(realpath($_SERVER["SCRIPT_FILENAME"])));

switch($testdir){
case "connect":
    if (!$orch->ping()) {
        exit("skip these tests in $testdir require Mongo Orchestration");
    }
    break;
}

CLEANUP();
