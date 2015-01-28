<?php

require __DIR__ . "/" . "basic-skipif.inc";
$parsed = parse_url(MONGODB_STANDALONE_X509_URI);

if (!isset($parsed["host"])) {
    exit("skip cannot parse uri: '" . MONGODB_STANDALONE_X509_URI. "' maybe x509 server not available?");
}

