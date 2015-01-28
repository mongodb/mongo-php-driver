<?php

require __DIR__ . "/" . "basic-skipif.inc";
$parsed = parse_url(MONGODB_STANDALONE_PLAIN_URI);

if (!isset($parsed["host"])) {
    exit("skip cannot parse uri: '" . MONGODB_STANDALONE_PLAIN_URI. "' maybe PLAIN server not available?");
}

