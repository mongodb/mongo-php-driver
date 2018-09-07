--TEST--
Test for PHPC-1266: Empty deeply nested BSON document causes unallocated memory writes
--FILE--
<?php
$a = <<<ENDJSON
{
    "value" : {     
        "payload" : {
            "PayloadMasterDataMeteringPointPartyEvent" : {
                "MeteringPointPartyDetailMeteringPointPartyCharacteristic" : {
                    "AdministrativePartyMPAdministrativeParty" : [
                        {
                            "AdministrativePartyAddressLocationAddress" : {
                                "StreetCode" : {
                                }
                            }
                        }
                    ]
                }
            }
        }
    }
}   
ENDJSON;

$bson = MongoDB\BSON\fromJSON($a);
var_dump(MongoDB\BSON\toPHP($bson));
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["value"]=>
  object(stdClass)#%d (%d) {
    ["payload"]=>
    object(stdClass)#%d (%d) {
      ["PayloadMasterDataMeteringPointPartyEvent"]=>
      object(stdClass)#%d (%d) {
        ["MeteringPointPartyDetailMeteringPointPartyCharacteristic"]=>
        object(stdClass)#%d (%d) {
          ["AdministrativePartyMPAdministrativeParty"]=>
          array(%d) {
            [0]=>
            object(stdClass)#%d (%d) {
              ["AdministrativePartyAddressLocationAddress"]=>
              object(stdClass)#%d (%d) {
                ["StreetCode"]=>
                object(stdClass)#%d (%d) {
                }
              }
            }
          }
        }
      }
    }
  }
}
===DONE===
