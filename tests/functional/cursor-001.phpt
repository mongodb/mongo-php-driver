--TEST--
Sorting single field, ascending, using the Cursor Iterator
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
    'sort' => array('username' => 1),
    'limit' => 104,
));

$cursor = $manager->executeQuery(NS, $query);

foreach ($cursor as $document) {
    echo $document->username . "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
aaliyah.kertzmann
aaron89
abbott.alden
abbott.flo
abby76
abernathy.adrienne
abernathy.audrey
abner.kreiger
aboehm
abshire.icie
abshire.jazlyn
adams.delta
adolph20
adonis.schamberger
agleason
ahartmann
ahettinger
akreiger
al.cormier
al97
albin95
alda.murray
alden.blanda
alessandra76
alex73
alexa01
alfred.ritchie
alia07
alia72
alize.hegmann
allie48
alta.sawayn
alvena.pacocha
alvis22
alycia48
amalia84
amely01
amos.corkery
amos78
anahi95
anais.feest
anais58
andreanne.steuber
angela.dickinson
angelina.bartoletti
angelina31
aniyah.franecki
annalise40
antoinette.gaylord
antoinette.weissnat
aoberbrunner
apacocha
apollich
ara92
arch44
arely.ryan
armstrong.clara
armstrong.gordon
arnold.kiehn
arvel.hilll
asatterfield
aschuppe
ashlynn71
ashlynn85
ashton.o'kon
austen03
austen47
austin67
awintheiser
awyman
ayana.brakus
bailey.mertz
bailey.sarina
balistreri.donald
barrett.prohaska
bartell.susie
bashirian.lina
bayer.ova
baylee.maggio
bbernier
bblick
beahan.oleta
beatty.layne
beatty.myrtis
beau49
beaulah.mann
bechtelar.nadia
becker.theron
beer.mossie
beer.roselyn
benedict.johnson
berge.enoch
bergnaum.roberto
bernardo.mccullough
bernardo52
bernhard.margaretta
bernie.morissette
bethel20
betty09
bins.aliyah
bins.laisha
bjori
blanda.danielle
blanda.irving
===DONE===
