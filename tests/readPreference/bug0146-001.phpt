--TEST--
PHPC-146: ReadPreference primaryPreferred and secondary swapped (OP_QUERY)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('STANDALONE_30'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE_30);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(array('my' => 'document'));
$manager->executeBulkWrite(NS, $bulk);

$rps = array(
    MongoDB\Driver\ReadPreference::RP_PRIMARY,
    MongoDB\Driver\ReadPreference::RP_PRIMARY_PREFERRED,
    MongoDB\Driver\ReadPreference::RP_SECONDARY,
    MongoDB\Driver\ReadPreference::RP_SECONDARY_PREFERRED,
    MongoDB\Driver\ReadPreference::RP_NEAREST,
);

foreach($rps as $r) {
    $rp = new MongoDB\Driver\ReadPreference($r);
    $cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array("my" => "query")), $rp);
    var_dump($cursor);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\Cursor)#%d (%d) {
  ["database"]=>
  string(6) "phongo"
  ["collection"]=>
  string(26) "readPreference_bug0146_001"
  ["query"]=>
  object(MongoDB\Driver\Query)#%d (%d) {
    ["filter"]=>
    object(stdClass)#%d (%d) {
      ["my"]=>
      string(5) "query"
    }
    ["options"]=>
    object(stdClass)#%d (%d) {
    }
    ["readConcern"]=>
    NULL
  }
  ["command"]=>
  NULL
  ["readPreference"]=>
  object(MongoDB\Driver\ReadPreference)#%d (%d) {
    ["mode"]=>
    string(7) "primary"
  }
  ["isDead"]=>
  bool(true)
  ["currentIndex"]=>
  int(0)
  ["currentDocument"]=>
  NULL
  ["server"]=>
  object(MongoDB\Driver\Server)#%d (%d) {
    %a
  }
}
object(MongoDB\Driver\Cursor)#%d (%d) {
  ["database"]=>
  string(6) "phongo"
  ["collection"]=>
  string(26) "readPreference_bug0146_001"
  ["query"]=>
  object(MongoDB\Driver\Query)#%d (%d) {
    ["filter"]=>
    object(stdClass)#%d (%d) {
      ["my"]=>
      string(5) "query"
    }
    ["options"]=>
    object(stdClass)#%d (%d) {
    }
    ["readConcern"]=>
    NULL
  }
  ["command"]=>
  NULL
  ["readPreference"]=>
  object(MongoDB\Driver\ReadPreference)#%d (%d) {
    ["mode"]=>
    string(16) "primaryPreferred"
  }
  ["isDead"]=>
  bool(true)
  ["currentIndex"]=>
  int(0)
  ["currentDocument"]=>
  NULL
  ["server"]=>
  object(MongoDB\Driver\Server)#%d (%d) {
    %a
  }
}
object(MongoDB\Driver\Cursor)#%d (%d) {
  ["database"]=>
  string(6) "phongo"
  ["collection"]=>
  string(26) "readPreference_bug0146_001"
  ["query"]=>
  object(MongoDB\Driver\Query)#%d (%d) {
    ["filter"]=>
    object(stdClass)#%d (%d) {
      ["my"]=>
      string(5) "query"
    }
    ["options"]=>
    object(stdClass)#%d (%d) {
    }
    ["readConcern"]=>
    NULL
  }
  ["command"]=>
  NULL
  ["readPreference"]=>
  object(MongoDB\Driver\ReadPreference)#%d (%d) {
    ["mode"]=>
    string(9) "secondary"
  }
  ["isDead"]=>
  bool(true)
  ["currentIndex"]=>
  int(0)
  ["currentDocument"]=>
  NULL
  ["server"]=>
  object(MongoDB\Driver\Server)#%d (%d) {
    %a
  }
}
object(MongoDB\Driver\Cursor)#%d (%d) {
  ["database"]=>
  string(6) "phongo"
  ["collection"]=>
  string(26) "readPreference_bug0146_001"
  ["query"]=>
  object(MongoDB\Driver\Query)#%d (%d) {
    ["filter"]=>
    object(stdClass)#%d (%d) {
      ["my"]=>
      string(5) "query"
    }
    ["options"]=>
    object(stdClass)#%d (%d) {
    }
    ["readConcern"]=>
    NULL
  }
  ["command"]=>
  NULL
  ["readPreference"]=>
  object(MongoDB\Driver\ReadPreference)#%d (%d) {
    ["mode"]=>
    string(18) "secondaryPreferred"
  }
  ["isDead"]=>
  bool(true)
  ["currentIndex"]=>
  int(0)
  ["currentDocument"]=>
  NULL
  ["server"]=>
  object(MongoDB\Driver\Server)#%d (%d) {
    %a
  }
}
object(MongoDB\Driver\Cursor)#%d (%d) {
  ["database"]=>
  string(6) "phongo"
  ["collection"]=>
  string(26) "readPreference_bug0146_001"
  ["query"]=>
  object(MongoDB\Driver\Query)#%d (%d) {
    ["filter"]=>
    object(stdClass)#%d (%d) {
      ["my"]=>
      string(5) "query"
    }
    ["options"]=>
    object(stdClass)#%d (%d) {
    }
    ["readConcern"]=>
    NULL
  }
  ["command"]=>
  NULL
  ["readPreference"]=>
  object(MongoDB\Driver\ReadPreference)#%d (%d) {
    ["mode"]=>
    string(7) "nearest"
  }
  ["isDead"]=>
  bool(true)
  ["currentIndex"]=>
  int(0)
  ["currentDocument"]=>
  NULL
  ["server"]=>
  object(MongoDB\Driver\Server)#%d (%d) {
    %a
  }
}
===DONE===
