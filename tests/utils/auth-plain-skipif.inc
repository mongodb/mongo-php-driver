<?php

require __DIR__ . "/" . "basic-skipif.inc";
$parsed = parse_url(STANDALONE_PLAIN);

if (!isset($parsed["host"])) {
    exit("skip cannot parse uri: '" . STANDALONE_PLAIN. "' maybe PLAIN server not available?");
}

