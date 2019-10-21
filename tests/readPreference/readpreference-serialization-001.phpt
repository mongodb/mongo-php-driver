--TEST--
MongoDB\Driver\ReadPreference serialization
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY_PREFERRED),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY_PREFERRED),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_NEAREST),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, []),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, [['dc' => 'ny']]),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, [['dc' => 'ny'], ['dc' => 'sf', 'use' => 'reporting'], []]),
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, null, ['maxStalenessSeconds' => 1000]),
];

foreach ($tests as $test) {
    var_dump($test);
    var_dump($test instanceof Serializable);
    echo $s = serialize($test), "\n";
    var_dump(unserialize($s));
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "primary"
}
bool(true)
C:29:"MongoDB\Driver\ReadPreference":31:{a:1:{s:4:"mode";s:7:"primary";}}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "primary"
}

object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(16) "primaryPreferred"
}
bool(true)
C:29:"MongoDB\Driver\ReadPreference":41:{a:1:{s:4:"mode";s:16:"primaryPreferred";}}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(16) "primaryPreferred"
}

object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
}
bool(true)
C:29:"MongoDB\Driver\ReadPreference":33:{a:1:{s:4:"mode";s:9:"secondary";}}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
}

object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(18) "secondaryPreferred"
}
bool(true)
C:29:"MongoDB\Driver\ReadPreference":43:{a:1:{s:4:"mode";s:18:"secondaryPreferred";}}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(18) "secondaryPreferred"
}

object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "nearest"
}
bool(true)
C:29:"MongoDB\Driver\ReadPreference":31:{a:1:{s:4:"mode";s:7:"nearest";}}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(7) "nearest"
}

object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
}
bool(true)
C:29:"MongoDB\Driver\ReadPreference":33:{a:1:{s:4:"mode";s:9:"secondary";}}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
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
}
bool(true)
C:29:"MongoDB\Driver\ReadPreference":78:{a:2:{s:4:"mode";s:9:"secondary";s:4:"tags";a:1:{i:0;a:1:{s:2:"dc";s:2:"ny";}}}}
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
}
bool(true)
C:29:"MongoDB\Driver\ReadPreference":142:{a:2:{s:4:"mode";s:9:"secondary";s:4:"tags";a:3:{i:0;a:1:{s:2:"dc";s:2:"ny";}i:1;a:2:{s:2:"dc";s:2:"sf";s:3:"use";s:9:"reporting";}i:2;a:0:{}}}}
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
}

object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["maxStalenessSeconds"]=>
  int(1000)
}
bool(true)
C:29:"MongoDB\Driver\ReadPreference":67:{a:2:{s:4:"mode";s:9:"secondary";s:19:"maxStalenessSeconds";i:1000;}}
object(MongoDB\Driver\ReadPreference)#%d (%d) {
  ["mode"]=>
  string(9) "secondary"
  ["maxStalenessSeconds"]=>
  int(1000)
}

===DONE===
