--TEST--
MongoDB\Driver\ReadPreference serialization (__serialize and __unserialize)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY_PREFERRED),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY_PREFERRED),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::NEAREST),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, []),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'ny']]),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, [['dc' => 'ny'], ['dc' => 'sf', 'use' => 'reporting'], []]),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['maxStalenessSeconds' => 1000]),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::SECONDARY, null, ['hedge' => ['enabled' => true]]),
];

foreach ($tests as $test) {
    var_dump($test);
    echo $s = serialize($test), "\n";
    var_dump(unserialize($s));
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: MongoDB\Driver\ReadPreference::__construct(): The "hedge" option is deprecated as of MongoDB 8.0 and will be removed in a future release in %s
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "primary"
  ["tags"]=>
  NULL
  ["maxStalenessSeconds"]=>
  int(-1)
  ["hedge"]=>
  NULL
}
O:29:"MongoDB\Driver\ReadPreference":4:{s:4:"mode";s:7:"primary";s:4:"tags";N;s:19:"maxStalenessSeconds";i:-1;s:5:"hedge";N;}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "primary"
  ["tags"]=>
  NULL
  ["maxStalenessSeconds"]=>
  int(-1)
  ["hedge"]=>
  NULL
}

object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(16) "primaryPreferred"
  ["tags"]=>
  NULL
  ["maxStalenessSeconds"]=>
  int(-1)
  ["hedge"]=>
  NULL
}
O:29:"MongoDB\Driver\ReadPreference":4:{s:4:"mode";s:16:"primaryPreferred";s:4:"tags";N;s:19:"maxStalenessSeconds";i:-1;s:5:"hedge";N;}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(16) "primaryPreferred"
  ["tags"]=>
  NULL
  ["maxStalenessSeconds"]=>
  int(-1)
  ["hedge"]=>
  NULL
}

object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  NULL
  ["maxStalenessSeconds"]=>
  int(-1)
  ["hedge"]=>
  NULL
}
O:29:"MongoDB\Driver\ReadPreference":4:{s:4:"mode";s:9:"secondary";s:4:"tags";N;s:19:"maxStalenessSeconds";i:-1;s:5:"hedge";N;}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  NULL
  ["maxStalenessSeconds"]=>
  int(-1)
  ["hedge"]=>
  NULL
}

object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(18) "secondaryPreferred"
  ["tags"]=>
  NULL
  ["maxStalenessSeconds"]=>
  int(-1)
  ["hedge"]=>
  NULL
}
O:29:"MongoDB\Driver\ReadPreference":4:{s:4:"mode";s:18:"secondaryPreferred";s:4:"tags";N;s:19:"maxStalenessSeconds";i:-1;s:5:"hedge";N;}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(18) "secondaryPreferred"
  ["tags"]=>
  NULL
  ["maxStalenessSeconds"]=>
  int(-1)
  ["hedge"]=>
  NULL
}

object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "nearest"
  ["tags"]=>
  NULL
  ["maxStalenessSeconds"]=>
  int(-1)
  ["hedge"]=>
  NULL
}
O:29:"MongoDB\Driver\ReadPreference":4:{s:4:"mode";s:7:"nearest";s:4:"tags";N;s:19:"maxStalenessSeconds";i:-1;s:5:"hedge";N;}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "nearest"
  ["tags"]=>
  NULL
  ["maxStalenessSeconds"]=>
  int(-1)
  ["hedge"]=>
  NULL
}

object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  NULL
  ["maxStalenessSeconds"]=>
  int(-1)
  ["hedge"]=>
  NULL
}
O:29:"MongoDB\Driver\ReadPreference":4:{s:4:"mode";s:9:"secondary";s:4:"tags";N;s:19:"maxStalenessSeconds";i:-1;s:5:"hedge";N;}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  NULL
  ["maxStalenessSeconds"]=>
  int(-1)
  ["hedge"]=>
  NULL
}

object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  array(1) {
    [0]=>
    object(stdClass)#%d (%d) {
      ["dc"]=>
      string(2) "ny"
    }
  }
  ["maxStalenessSeconds"]=>
  int(-1)
  ["hedge"]=>
  NULL
}
O:29:"MongoDB\Driver\ReadPreference":4:{s:4:"mode";s:9:"secondary";s:4:"tags";a:1:{i:0;O:8:"stdClass":1:{s:2:"dc";s:2:"ny";}}s:19:"maxStalenessSeconds";i:-1;s:5:"hedge";N;}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  array(1) {
    [0]=>
    object(stdClass)#%d (%d) {
      ["dc"]=>
      string(2) "ny"
    }
  }
  ["maxStalenessSeconds"]=>
  int(-1)
  ["hedge"]=>
  NULL
}

object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  array(3) {
    [0]=>
    object(stdClass)#%d (%d) {
      ["dc"]=>
      string(2) "ny"
    }
    [1]=>
    object(stdClass)#%d (%d) {
      ["dc"]=>
      string(2) "sf"
      ["use"]=>
      string(9) "reporting"
    }
    [2]=>
    object(stdClass)#%d (%d) {
    }
  }
  ["maxStalenessSeconds"]=>
  int(-1)
  ["hedge"]=>
  NULL
}
O:29:"MongoDB\Driver\ReadPreference":4:{s:4:"mode";s:9:"secondary";s:4:"tags";a:3:{i:0;O:8:"stdClass":1:{s:2:"dc";s:2:"ny";}i:1;O:8:"stdClass":2:{s:2:"dc";s:2:"sf";s:3:"use";s:9:"reporting";}i:2;O:8:"stdClass":0:{}}s:19:"maxStalenessSeconds";i:-1;s:5:"hedge";N;}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  array(3) {
    [0]=>
    object(stdClass)#%d (%d) {
      ["dc"]=>
      string(2) "ny"
    }
    [1]=>
    object(stdClass)#%d (%d) {
      ["dc"]=>
      string(2) "sf"
      ["use"]=>
      string(9) "reporting"
    }
    [2]=>
    object(stdClass)#%d (%d) {
    }
  }
  ["maxStalenessSeconds"]=>
  int(-1)
  ["hedge"]=>
  NULL
}

object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  NULL
  ["maxStalenessSeconds"]=>
  int(1000)
  ["hedge"]=>
  NULL
}
O:29:"MongoDB\Driver\ReadPreference":4:{s:4:"mode";s:9:"secondary";s:4:"tags";N;s:19:"maxStalenessSeconds";i:1000;s:5:"hedge";N;}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  NULL
  ["maxStalenessSeconds"]=>
  int(1000)
  ["hedge"]=>
  NULL
}

object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  NULL
  ["maxStalenessSeconds"]=>
  int(-1)
  ["hedge"]=>
  object(stdClass)#%d (%d) {
    ["enabled"]=>
    bool(true)
  }
}
O:29:"MongoDB\Driver\ReadPreference":4:{s:4:"mode";s:9:"secondary";s:4:"tags";N;s:19:"maxStalenessSeconds";i:-1;s:5:"hedge";O:8:"stdClass":1:{s:7:"enabled";b:1;}}

Deprecated: MongoDB\Driver\ReadPreference::__unserialize(): The "hedge" option is deprecated as of MongoDB 8.0 and will be removed in a future release in %s
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["tags"]=>
  NULL
  ["maxStalenessSeconds"]=>
  int(-1)
  ["hedge"]=>
  object(stdClass)#%d (%d) {
    ["enabled"]=>
    bool(true)
  }
}

===DONE===
