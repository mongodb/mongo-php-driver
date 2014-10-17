--TEST--
Sorting single field, descending
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
<?php require "tests/utils/fixtures-users.inc" ?>
--FILE--
<?php 
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Manager(MONGODB_URI);

$query = new MongoDB\Query(array(), array(
    'projection' => array('_id' => 0, 'username' => 1),
    'sort' => array('username' => -1),
));

$cursor = $manager->executeQuery(NS, $query);

foreach ($cursor as $document) {
    echo $document['username'] . "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
zstanton
ywyman
ypredovic
yost.magali
yost.ari
yhudson
yasmin55
xgibson
wwilkinson
wschimmel
wschaefer
witting.chris
wisozk.cortez
west.jude
walter.lester
walker.alec
vwaters
vrolfson
von.britney
thalia22
talon74
tabitha.mohr
swyman
ssteuber
spencer.darrel
shyann28
sawayn.catharine
rodger.raynor
rau.brent
qschiller
qkunze
pacocha.quentin
orn.katelyn
ohowe
odell96
o'conner.arthur
npurdy
ngoldner
nayeli.vandervort
myra43
mraz.marcelina
marlene95
marietta.swift
mante.ashlee
lucio20
lubowitz.colleen
lonnie10
lonnie.little
lmckenzie
koch.sophia
khills
kelvin.jakubowski
keebler.rupert
kattie12
justina63
jschinner
jfeest
jessy16
jessika.schmeler
jazmyne63
javier13
javier.volkman
janelle93
jamaal.cassin
jailyn62
isac13
helene.o'connell
hartmann.dedrick
haley.krystel
gladyce88
german.leffler
fosinski
ford85
farrell.asha
ervin.carter
ernser.addison
eliseo49
eldora.steuber
edibbert
ebony59
ebert.cordie
dwolf
doyle.nelle
dhudson
dereck.ward
cronin.clint
crona.jaclyn
chance.conroy
cartwright.garland
camilla20
bergnaum.roberto
beahan.oleta
bartell.susie
balistreri.donald
ashton.o'kon
aschuppe
armstrong.gordon
andreanne.steuber
alda.murray
abernathy.audrey
===DONE===
