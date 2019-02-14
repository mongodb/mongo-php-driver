<?php

require_once __DIR__ . '/basic.inc';
require_once __DIR__ . '/skipif.php';

if ( ! extension_loaded('mongodb')) {
    exit('skip mongodb extension is not loaded');
}

/* Force date.timezone to be "UTC" */
date_default_timezone_set("UTC");

set_error_handler(function($errno, $errstr, $errfile, $errline) {
    exit(sprintf('skip %s: %s @ %s:%d', errno_as_string($errno), $errstr, $errfile, $errline));
});

set_exception_handler(function($e) {
    exit(sprintf('skip %s(%d): %s @ %s:%d', get_class($e), $e->getCode(), $e->getMessage(), $e->getFile(), $e->getLine()));
});

register_shutdown_function(function() {
    $lastError = error_get_last();

    if ($lastError !== null) {
        exit(sprintf('skip %s: %s', errno_as_string($lastError['type']), $lastError['message']));
    }
});
