--TEST--
PHPC-431: Segfault when using Manager through singleton class
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
use MongoDB\Driver\Manager;
use MongoDB\Driver\Query;
use MongoDB\Driver\ReadPreference;
 
require_once __DIR__ . "/../utils/basic.inc";

 
class Database {
    private $Database;
    private static $Instance;
 
    public function __construct() {
        $Manager = new Manager(URI, array(), array());
        $this->Database = $Manager;
    }
 
    public static function getInstance() {
        if (static::$Instance == null) {
            static::$Instance = new Database();
        }
 
        return static::$Instance;
    }
 
    public function query($scheme, $query) {
        return $this->Database->executeQuery($scheme, $query, new ReadPreference(ReadPreference::RP_PRIMARY));
    }
}
 
class App {
    public function run() {
        $db = Database::getInstance();
        
        $query = new Query(array());
        $cursor = $db->query(DATABASE_NAME . ".scheme_info", $query);
 
        foreach ($cursor as $document) {
            echo $document->value;
        }
 
        $query = new Query(array());
        $cursor = $db->query(DATABASE_NAME . ".domain", $query);
 
        foreach ($cursor as $document) {
            echo $document->hostname;
        }
    }
}
 
$App = new App();
$App->run();
echo "All done\n";
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
All done
===DONE===
