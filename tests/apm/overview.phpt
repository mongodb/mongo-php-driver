--TEST--
PHPC-349: APM Specification
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
<?php skip_if_server_version('<', '3.2'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$m = new MongoDB\Driver\Manager(URI);

class MySubscriber implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted( \MongoDB\Driver\Monitoring\CommandStartedEvent $event )
    {
        echo "started:\n";
        var_dump( $event );
    }

    public function commandSucceeded( \MongoDB\Driver\Monitoring\CommandSucceededEvent $event )
    {
        echo "succeeded:\n";
        var_dump( $event );
    }

    public function commandFailed( \MongoDB\Driver\Monitoring\CommandFailedEvent $event )
    {
        echo "failed:\n";
        var_dump( $event );
    }
}

MongoDB\Driver\Monitoring\addSubscriber( new MySubscriber() );
drop_collection(URI, DATABASE_NAME, COLLECTION_NAME);

$d = 12345678;

$bw = new MongoDB\Driver\BulkWrite( [ 'ordered' => false ] );
$_id = $bw->insert( [ 'decimal' => $d ] );
$r = $m->executeBulkWrite( DATABASE_NAME . '.' . COLLECTION_NAME, $bw );

$query = new MongoDB\Driver\Query( [] );
$cursor = $m->executeQuery( DATABASE_NAME . '.' . COLLECTION_NAME, $query );
var_dump( $cursor->toArray() );
?>
--EXPECTF--
started:
object(MongoDB\Driver\Monitoring\CommandStartedEvent)#%d (%d) {
  ["command"]=>
  object(stdClass)#%d (%d) {
    ["drop"]=>
    string(12) "apm_overview"%A
  }
  ["commandName"]=>
  string(4) "drop"
  ["databaseName"]=>
  string(6) "phongo"
  ["operationId"]=>
  string(%d) "%s"
  ["requestId"]=>
  string(%d) "%s"
  ["server"]=>
  object(MongoDB\Driver\Server)#%d (%d) {
    %a
  }
}
failed:
object(MongoDB\Driver\Monitoring\CommandFailedEvent)#%d (%d) {
  ["commandName"]=>
  string(4) "drop"
  ["durationMicros"]=>
  int(%d)
  ["error"]=>
  object(MongoDB\Driver\Exception\ServerException)#%d (%d) {
    ["message":protected]=>
    string(12) "ns not found"
    ["string":"Exception":private]=>
    string(0) ""
    ["code":protected]=>
    int(26)
    ["file":protected]=>
    string(%d) "%stests/%s"
    ["line":protected]=>
    int(%d)
    ["trace":"Exception":private]=>
    %a
    ["previous":"Exception":private]=>
    NULL
  }
  ["operationId"]=>
  string(%d) "%s"
  ["reply"]=>
  object(stdClass)#%d (%d) {%A
    ["ok"]=>
    float(0)
    ["errmsg"]=>
    string(12) "ns not found"
    ["code"]=>
    int(26)%A
  }
  ["requestId"]=>
  string(%d) "%s"
  ["server"]=>
  object(MongoDB\Driver\Server)#%d (%d) {
    %a
  }
}
started:
object(MongoDB\Driver\Monitoring\CommandStartedEvent)#%d (%d) {
  ["command"]=>
  object(stdClass)#%d (%d) {
    ["insert"]=>
    string(12) "apm_overview"
    ["ordered"]=>
    bool(false)%A
    ["documents"]=>
    array(1) {
      [0]=>
      object(stdClass)#%d (%d) {
        ["decimal"]=>
        int(12345678)
        ["_id"]=>
        object(MongoDB\BSON\ObjectId)#%d (%d) {
          ["oid"]=>
          string(24) "%s"
        }
      }
    }
  }
  ["commandName"]=>
  string(6) "insert"
  ["databaseName"]=>
  string(6) "phongo"
  ["operationId"]=>
  string(%d) "%s"
  ["requestId"]=>
  string(%d) "%s"
  ["server"]=>
  object(MongoDB\Driver\Server)#%d (%d) {
    %a
  }
}
succeeded:
object(MongoDB\Driver\Monitoring\CommandSucceededEvent)#%d (%d) {
  ["commandName"]=>
  string(6) "insert"
  ["durationMicros"]=>
  int(%d)
  ["operationId"]=>
  string(%d) "%s"
  ["reply"]=>
  object(stdClass)#%d (%d) {
    %a
  }
  ["requestId"]=>
  string(%d) "%s"
  ["server"]=>
  object(MongoDB\Driver\Server)#%d (%d) {
    %a
  }
}
started:
object(MongoDB\Driver\Monitoring\CommandStartedEvent)#%d (%d) {
  ["command"]=>
  object(stdClass)#%d (%d) {
    ["find"]=>
    string(12) "apm_overview"
    ["filter"]=>
    object(stdClass)#%d (%d) {
    }%A
  }
  ["commandName"]=>
  string(4) "find"
  ["databaseName"]=>
  string(6) "phongo"
  ["operationId"]=>
  string(%d) "%s"
  ["requestId"]=>
  string(%d) "%s"
  ["server"]=>
  object(MongoDB\Driver\Server)#%d (%d) {
    %a
  }
}
succeeded:
object(MongoDB\Driver\Monitoring\CommandSucceededEvent)#%d (%d) {
  ["commandName"]=>
  string(4) "find"
  ["durationMicros"]=>
  int(%d)
  ["operationId"]=>
  string(%d) "%s"
  ["reply"]=>
  object(stdClass)#%d (%d) {
    %a
  }
  ["requestId"]=>
  string(%d) "%s"
  ["server"]=>
  object(MongoDB\Driver\Server)#%d (%d) {
    %a
  }
}
array(%d) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    object(MongoDB\BSON\ObjectId)#%d (%d) {
      ["oid"]=>
      string(24) "%s"
    }
    ["decimal"]=>
    int(12345678)
  }
}
