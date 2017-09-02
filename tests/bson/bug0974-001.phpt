--TEST--
PHPC-974: Converting JSON to BSON to PHP introduces gaps in array indexes
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    '{"myArray": [{"$oid": "2017-06-13T11:21:26.906Z"}]}',
    '[1,2,3,{"$date": "2017-06-13T11:21:26.906Z"}]',
    '[{"$oid": "55f2b3f1f657b3fa97c9c0a2"},{"$date": "2017-06-13T11:21:26.906Z"}]',
];

foreach ($tests as $test) {
    $bson = fromJSON($test);
    var_dump(toPHP($bson));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["myArray"]=>
  array(1) {
    [0]=>
    object(MongoDB\BSON\ObjectId)#%d (%d) {
      ["oid"]=>
      string(24) "201700601301102102609060"
    }
  }
}
object(stdClass)#%d (%d) {
  [%r(0|"0")%r]=>
  int(1)
  [%r(1|"1")%r]=>
  int(2)
  [%r(2|"2")%r]=>
  int(3)
  [%r(3|"3")%r]=>
  object(MongoDB\BSON\UTCDateTime)#%d (%d) {
    ["milliseconds"]=>
    string(13) "1497352886906"
  }
}
object(stdClass)#3 (2) {
  [%r(0|"0")%r]=>
  object(MongoDB\BSON\ObjectId)#%d (%d) {
    ["oid"]=>
    string(24) "55f2b3f1f657b3fa97c9c0a2"
  }
  [%r(1|"1")%r]=>
  object(MongoDB\BSON\UTCDateTime)#%d (%d) {
    ["milliseconds"]=>
    string(13) "1497352886906"
  }
}
===DONE===
