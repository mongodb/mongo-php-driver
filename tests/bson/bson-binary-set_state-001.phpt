--TEST--
MongoDB\BSON\Binary::__set_state()
--FILE--
<?php

$tests = [
    ['foobar', MongoDB\BSON\Binary::TYPE_GENERIC],
    ['', MongoDB\BSON\Binary::TYPE_GENERIC],
    ["\0foo", MongoDB\BSON\Binary::TYPE_GENERIC],
    [hex2bin('123e4567e89b12d3a456426655440000'), MongoDB\BSON\Binary::TYPE_UUID],
    [md5('foobar', true), MongoDB\BSON\Binary::TYPE_MD5],
];

foreach ($tests as $test) {
    list($data, $type) = $test;

    var_export(MongoDB\BSON\Binary::__set_state([
        'data' => $data,
        'type' => $type,
    ]));
    echo "\n\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
MongoDB\BSON\Binary::__set_state(array(
%w'data' => 'foobar',
%w'type' => 0,
))

MongoDB\BSON\Binary::__set_state(array(
%w'data' => '',
%w'type' => 0,
))

MongoDB\BSON\Binary::__set_state(array(
%w'data' => '' . "\0" . 'foo',
%w'type' => 0,
))

MongoDB\BSON\Binary::__set_state(array(
%w'data' => '>Egè›Ó¤VBfUD' . "\0" . '' . "\0" . '',
%w'type' => 4,
))

MongoDB\BSON\Binary::__set_state(array(
%w'data' => '8Xö"0¬<‘_0fCÆ?',
%w'type' => 5,
))

===DONE===
