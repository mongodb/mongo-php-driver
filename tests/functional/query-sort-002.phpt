--TEST--
Sorting single field, descending
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
loadFixtures($manager);

$query = new MongoDB\Driver\Query(array(), array(
    'projection' => array('_id' => 0, 'username' => 1),
    'sort' => array('username' => -1),
    'limit' => 100,
));

$cursor = $manager->executeQuery(NS, $query);

foreach ($cursor as $document) {
    echo $document->username . "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
zulauf.amaya
zstanton
zoe41
zieme.noemi
ziemann.webster
zheathcote
zella78
zboyle
zachery33
yyost
ywyman
ywiza
ypredovic
yost.magali
yost.ari
ylarkin
yklein
yhudson
yfritsch
ycole
yasmine.lowe
yasmin55
xrodriguez
xkohler
xhermann
xgutmann
xgibson
xcassin
wwilkinson
wunsch.mose
wschimmel
wschaefer
wpacocha
wolff.caroline
wkertzmann
wiza.carmel
witting.walker
witting.chris
wisozk.cortez
winnifred08
wilson.white
willms.amari
will.lamont
will.jerod
will.edwina
wilfred.feil
wilderman.sophia
wiegand.blanche
west.jude
west.cristobal
weimann.tillman
webster70
webster48
watson70
warren.feest
walton33
walter.norval
walter.lester
walsh.vincenza
walker.alec
wade91
vwaters
vvolkman
vschulist
vrolfson
vpfeffer
vorn
von.britney
vivianne.macejkovic
veum.tyrell
vesta.ritchie
verda93
vena.schumm
velma37
velda.wehner
veffertz
vdickinson
vconn
vbraun
vborer
vbins
vandervort.ezekiel
van.ruecker
uzieme
uwisoky
usmith
uschumm
uschmeler
urban24
upton.zackery
unique.pagac
una.larkin
umraz
ullrich.layne
ulises44
ulises.beatty
ulesch
ukovacek
ujenkins
uhansen
===DONE===
