--TEST--
MongoDB\Driver\ReadPreference#002: PHPC-146
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);
$manager->executeInsert(NS, array("my" => "document"));
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
object(MongoDB\Driver\Result)#%d (%d) {
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
    array(1) {
      ["$query"]=>
      object(stdClass)#%d (%d) {
        ["my"]=>
        string(5) "query"
      }
    }
    ["fields"]=>
    array(0) {
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
    string(25) "phongo.readPreference_002"
  }
  ["firstBatch"]=>
  NULL
  ["hint"]=>
  int(1)
  ["is_command_cursor"]=>
  bool(false)
}
object(MongoDB\Driver\Result)#%d (%d) {
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
    array(2) {
      ["$query"]=>
      object(stdClass)#%d (%d) {
        ["my"]=>
        string(5) "query"
      }
      ["$readPreference"]=>
      object(stdClass)#%d (%d) {
        ["mode"]=>
        string(16) "primaryPreferred"
        ["tags"]=>
        array(0) {
        }
      }
    }
    ["fields"]=>
    array(0) {
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
    string(25) "phongo.readPreference_002"
  }
  ["firstBatch"]=>
  NULL
  ["hint"]=>
  int(1)
  ["is_command_cursor"]=>
  bool(false)
}
object(MongoDB\Driver\Result)#%d (%d) {
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
    array(2) {
      ["$query"]=>
      object(stdClass)#%d (%d) {
        ["my"]=>
        string(5) "query"
      }
      ["$readPreference"]=>
      object(stdClass)#%d (%d) {
        ["mode"]=>
        string(9) "secondary"
        ["tags"]=>
        array(0) {
        }
      }
    }
    ["fields"]=>
    array(0) {
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
    string(25) "phongo.readPreference_002"
  }
  ["firstBatch"]=>
  NULL
  ["hint"]=>
  int(1)
  ["is_command_cursor"]=>
  bool(false)
}
object(MongoDB\Driver\Result)#%d (%d) {
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
    array(2) {
      ["$query"]=>
      object(stdClass)#%d (%d) {
        ["my"]=>
        string(5) "query"
      }
      ["$readPreference"]=>
      object(stdClass)#%d (%d) {
        ["mode"]=>
        string(18) "secondaryPreferred"
        ["tags"]=>
        array(0) {
        }
      }
    }
    ["fields"]=>
    array(0) {
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
    string(25) "phongo.readPreference_002"
  }
  ["firstBatch"]=>
  NULL
  ["hint"]=>
  int(1)
  ["is_command_cursor"]=>
  bool(false)
}
object(MongoDB\Driver\Result)#%d (%d) {
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
    array(2) {
      ["$query"]=>
      object(stdClass)#%d (%d) {
        ["my"]=>
        string(5) "query"
      }
      ["$readPreference"]=>
      object(stdClass)#%d (%d) {
        ["mode"]=>
        string(7) "nearest"
        ["tags"]=>
        array(0) {
        }
      }
    }
    ["fields"]=>
    array(0) {
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
    string(25) "phongo.readPreference_002"
  }
  ["firstBatch"]=>
  NULL
  ["hint"]=>
  int(1)
  ["is_command_cursor"]=>
  bool(false)
}
===DONE===
