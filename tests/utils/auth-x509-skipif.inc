<?php

require __DIR__ . "/" . "basic-skipif.inc";
$parsed = parse_url(STANDALONE_X509);

if (!isset($parsed["host"])) {
    exit("skip cannot parse uri: '" . STANDALONE_X509 . "' maybe x509 server not available?");
}

