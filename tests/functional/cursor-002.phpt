--TEST--
Sorting single field, ascending, using the Cursor Iterator
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
<?php require "tests/utils/fixtures-users.inc" ?>
--FILE--
<?php 
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Driver\Manager(MONGODB_URI);

$query = new MongoDB\Driver\Query(array(), array(
    'projection' => array('_id' => 0, 'username' => 1),
    'sort' => array('username' => 1),
));

$qr = $manager->executeQuery(NS, $query);
$cursor = $qr->getIterator();

var_dump(
    $cursor->getBatchSize(),
    $cursor->setBatchSize(15),
    $cursor->getBatchSize(),
    $cursor->isDead()
);



for($j=0, $cursor->rewind(); $cursor->valid(); $cursor->next(), $j++) {
    $document = $cursor->current();
    echo $document['username'] . "\n";

    if ($j == 10) {
        var_dump($cursor);
    }
}

var_dump(
    $cursor->getBatchSize(),
    $cursor->setBatchSize(25),
    $cursor->getBatchSize(),
    $cursor->isDead()
);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
int(0)
int(0)
int(15)
bool(true)
abernathy.audrey
alda.murray
andreanne.steuber
armstrong.gordon
aschuppe
ashton.o'kon
balistreri.donald
bartell.susie
beahan.oleta
bergnaum.roberto
camilla20
object(MongoDB\Driver\Cursor)#%d (%d) {
  ["current_index"]=>
  int(10)
  ["result"]=>
  array(4) {
    ["cursor"]=>
    array(19) {
      ["stamp"]=>
      int(0)
      ["is_command"]=>
      bool(false)
      ["sent"]=>
      bool(true)
      ["done"]=>
      bool(false)
      ["failed"]=>
      bool(false)
      ["end_of_event"]=>
      bool(false)
      ["in_exhaust"]=>
      bool(false)
      ["redir_primary"]=>
      bool(false)
      ["has_fields"]=>
      bool(true)
      ["query"]=>
      array(2) {
        ["$orderby"]=>
        object(stdClass)#%d (%d) {
          ["username"]=>
          int(1)
        }
        ["$query"]=>
        object(stdClass)#%d (%d) {
        }
      }
      ["fields"]=>
      array(2) {
        ["_id"]=>
        int(0)
        ["username"]=>
        int(1)
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
      int(11)
      ["batch_size"]=>
      int(15)
      ["ns"]=>
      string(28) "phongo.functional_cursor_002"
      ["current_doc"]=>
      array(1) {
        ["username"]=>
        string(9) "camilla20"
      }
    }
    ["firstBatch"]=>
    array(1) {
      ["username"]=>
      string(16) "abernathy.audrey"
    }
    ["hint"]=>
    int(1)
    ["is_command_cursor"]=>
    bool(false)
  }
}
cartwright.garland
chance.conroy
crona.jaclyn
cronin.clint
dereck.ward
dhudson
doyle.nelle
dwolf
ebert.cordie
ebony59
edibbert
eldora.steuber
eliseo49
ernser.addison
ervin.carter
farrell.asha
ford85
fosinski
german.leffler
gladyce88
haley.krystel
hartmann.dedrick
helene.o'connell
isac13
jailyn62
jamaal.cassin
janelle93
javier.volkman
javier13
jazmyne63
jessika.schmeler
jessy16
jfeest
jschinner
justina63
kattie12
keebler.rupert
kelvin.jakubowski
khills
koch.sophia
lmckenzie
lonnie.little
lonnie10
lubowitz.colleen
lucio20
mante.ashlee
marietta.swift
marlene95
mraz.marcelina
myra43
nayeli.vandervort
ngoldner
npurdy
o'conner.arthur
odell96
ohowe
orn.katelyn
pacocha.quentin
qkunze
qschiller
rau.brent
rodger.raynor
sawayn.catharine
shyann28
spencer.darrel
ssteuber
swyman
tabitha.mohr
talon74
thalia22
von.britney
vrolfson
vwaters
walker.alec
walter.lester
west.jude
wisozk.cortez
witting.chris
wschaefer
wschimmel
wwilkinson
xgibson
yasmin55
yhudson
yost.ari
yost.magali
ypredovic
ywyman
zstanton
int(15)
int(15)
int(25)
bool(true)
===DONE===
