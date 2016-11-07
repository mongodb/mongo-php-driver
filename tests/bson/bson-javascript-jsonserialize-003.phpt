--TEST--
MongoDB\BSON\Javascript::jsonSerialize() with json_encode() (without scope)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$doc = ['foo' => new MongoDB\BSON\Javascript('function foo(bar) { return bar; }')];
$json = json_encode($doc);

/* Note: libbson currently does properly handle Javascript types. Conversion of
 * BSON to JSON yields a single code string value instead of a document with
 * "$code" and "$scope" fields. Likewise, "$code" and "$scope" fields are not
 * parsed when converting JSON to BSON. See CDRIVER-1335 for more info. */
echo toJSON(fromPHP($doc)), "\n";
echo $json, "\n";
var_dump(toPHP(fromJSON($json)));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
{ "foo" : "function foo(bar) { return bar; }" }
{"foo":{"$code":"function foo(bar) { return bar; }"}}
object(stdClass)#%d (%d) {
  ["foo"]=>
  object(stdClass)#%d (%d) {
    ["$code"]=>
    string(33) "function foo(bar) { return bar; }"
  }
}
===DONE===
