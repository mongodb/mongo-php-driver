<?php
$FILENAME = sys_get_temp_dir() . "/PHONGO-SERVERS.json";

$json = file_get_contents($FILENAME);
$servers = json_decode($json);
foreach($servers as $serverid => $uri) {
    printf("%-20s \t %s\n", $serverid, $uri);
}


