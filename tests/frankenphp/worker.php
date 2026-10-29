<?php

/*
 * FrankenPHP worker used by tests/frankenphp/restart-test.sh.
 *
 * FrankenPHP reboots all its PHP threads in-process when opcache_reset() is
 * called. The engine then runs GSHUTDOWN and MINIT again in the same process,
 * while libmongoc can only be initialized once per process. The instance
 * identifier changes every time the worker script runs, which lets the test
 * check that the engine was really restarted.
 */

$instance = bin2hex(random_bytes(8));

// Creating a Manager appends the driver handshake data, which aborts if
// libmongoc's process-wide state was released by a previous engine shutdown.
$manager = new MongoDB\Driver\Manager('mongodb://127.0.0.1:27017/');

$handler = static function () use ($instance): void {
    $path = parse_url($_SERVER['REQUEST_URI'], PHP_URL_PATH);

    if ($path === '/reset') {
        echo opcache_reset() ? 'reset' : 'reset-failed';

        return;
    }

    echo $instance;
};

while (frankenphp_handle_request($handler)) {
    gc_collect_cycles();
}
