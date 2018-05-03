<?php

require_once __DIR__ . '/basic.inc';
require_once __DIR__ . '/skipif.php';

if ( ! extension_loaded('mongodb')) {
    exit('skip mongodb extension is not loaded');
}

set_error_handler(function($errno, $errstr) {
    exit(sprintf('skip %s: %s', errno_as_string($errno), $errstr));
});

set_exception_handler(function($e) {
    exit(sprintf('skip %s(%d): %s', get_class($e), $e->getCode(), $e->getMessage()));
});

register_shutdown_function(function() {
    $lastError = error_get_last();

    if ($lastError !== null) {
        exit(sprintf('skip %s: %s', errno_as_string($lastError['type']), $lastError['message']));
    }
});
