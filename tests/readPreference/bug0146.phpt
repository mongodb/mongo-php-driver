--TEST--
PHPC-146: ReadPreference primaryPreferred and secondary swapped
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

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
  ["cursor"]=>
  array(18) {
    ["stamp"]=>
    int(0)
    ["is_command"]=>
    bool(false)
    ["sent"]=>
    bool(true)
    ["done"]=>
    bool(true)
    ["failed"]=>
    bool(false)
    ["end_of_event"]=>
    bool(true)
    ["in_exhaust"]=>
    bool(false)
    ["redir_primary"]=>
    bool(false)
    ["has_fields"]=>
    bool(false)
    ["query"]=>
    object(stdClass)#%d (1) {
      ["$query"]=>
      object(stdClass)#%d (%d) {
        ["my"]=>
        string(5) "query"
      }
    }
    ["fields"]=>
    object(stdClass)#%d (0) {
    }
    ["read_preference"]=>
    array(2) {
      ["mode"]=>
      int(1)
      ["tags"]=>
      array(0) {
      }
    }
    ["flags"]=>
    int(0)
    ["skip"]=>
    int(0)
    ["limit"]=>
    int(0)
    ["count"]=>
    int(1)
    ["batch_size"]=>
    int(0)
    ["ns"]=>
    string(%d) "%s"
  }
  ["server_id"]=>
  int(1)
}
object(MongoDB\Driver\Cursor)#%d (%d) {
  ["cursor"]=>
  array(18) {
    ["stamp"]=>
    int(0)
    ["is_command"]=>
    bool(false)
    ["sent"]=>
    bool(true)
    ["done"]=>
    bool(true)
    ["failed"]=>
    bool(false)
    ["end_of_event"]=>
    bool(true)
    ["in_exhaust"]=>
    bool(false)
    ["redir_primary"]=>
    bool(false)
    ["has_fields"]=>
    bool(false)
    ["query"]=>
    object(stdClass)#%d (%d) {
      ["$query"]=>
      object(stdClass)#%d (%d) {
        ["my"]=>
        string(5) "query"
      }
    }
    ["fields"]=>
    object(stdClass)#%d (0) {
    }
    ["read_preference"]=>
    array(2) {
      ["mode"]=>
      int(5)
      ["tags"]=>
      array(0) {
      }
    }
    ["flags"]=>
    int(0)
    ["skip"]=>
    int(0)
    ["limit"]=>
    int(0)
    ["count"]=>
    int(1)
    ["batch_size"]=>
    int(0)
    ["ns"]=>
    string(%d) "%s"
  }
  ["server_id"]=>
  int(1)
}
object(MongoDB\Driver\Cursor)#%d (%d) {
  ["cursor"]=>
  array(18) {
    ["stamp"]=>
    int(0)
    ["is_command"]=>
    bool(false)
    ["sent"]=>
    bool(true)
    ["done"]=>
    bool(true)
    ["failed"]=>
    bool(false)
    ["end_of_event"]=>
    bool(true)
    ["in_exhaust"]=>
    bool(false)
    ["redir_primary"]=>
    bool(false)
    ["has_fields"]=>
    bool(false)
    ["query"]=>
    object(stdClass)#%d (%d) {
      ["$query"]=>
      object(stdClass)#%d (%d) {
        ["my"]=>
        string(5) "query"
      }
    }
    ["fields"]=>
    object(stdClass)#%d (0) {
    }
    ["read_preference"]=>
    array(2) {
      ["mode"]=>
      int(2)
      ["tags"]=>
      array(0) {
      }
    }
    ["flags"]=>
    int(0)
    ["skip"]=>
    int(0)
    ["limit"]=>
    int(0)
    ["count"]=>
    int(1)
    ["batch_size"]=>
    int(0)
    ["ns"]=>
    string(%d) "%s"
  }
  ["server_id"]=>
  int(1)
}
object(MongoDB\Driver\Cursor)#%d (%d) {
  ["cursor"]=>
  array(18) {
    ["stamp"]=>
    int(0)
    ["is_command"]=>
    bool(false)
    ["sent"]=>
    bool(true)
    ["done"]=>
    bool(true)
    ["failed"]=>
    bool(false)
    ["end_of_event"]=>
    bool(true)
    ["in_exhaust"]=>
    bool(false)
    ["redir_primary"]=>
    bool(false)
    ["has_fields"]=>
    bool(false)
    ["query"]=>
    object(stdClass)#%d (%d) {
      ["$query"]=>
      object(stdClass)#%d (%d) {
        ["my"]=>
        string(5) "query"
      }
    }
    ["fields"]=>
    object(stdClass)#%d (0) {
    }
    ["read_preference"]=>
    array(2) {
      ["mode"]=>
      int(6)
      ["tags"]=>
      array(0) {
      }
    }
    ["flags"]=>
    int(0)
    ["skip"]=>
    int(0)
    ["limit"]=>
    int(0)
    ["count"]=>
    int(1)
    ["batch_size"]=>
    int(0)
    ["ns"]=>
    string(%d) "%s"
  }
  ["server_id"]=>
  int(1)
}
object(MongoDB\Driver\Cursor)#%d (%d) {
  ["cursor"]=>
  array(18) {
    ["stamp"]=>
    int(0)
    ["is_command"]=>
    bool(false)
    ["sent"]=>
    bool(true)
    ["done"]=>
    bool(true)
    ["failed"]=>
    bool(false)
    ["end_of_event"]=>
    bool(true)
    ["in_exhaust"]=>
    bool(false)
    ["redir_primary"]=>
    bool(false)
    ["has_fields"]=>
    bool(false)
    ["query"]=>
    object(stdClass)#%d (%d) {
      ["$query"]=>
      object(stdClass)#%d (%d) {
        ["my"]=>
        string(5) "query"
      }
    }
    ["fields"]=>
    object(stdClass)#%d (0) {
    }
    ["read_preference"]=>
    array(2) {
      ["mode"]=>
      int(10)
      ["tags"]=>
      array(0) {
      }
    }
    ["flags"]=>
    int(0)
    ["skip"]=>
    int(0)
    ["limit"]=>
    int(0)
    ["count"]=>
    int(1)
    ["batch_size"]=>
    int(0)
    ["ns"]=>
    string(%d) "%s"
  }
  ["server_id"]=>
  int(1)
}
===DONE===
