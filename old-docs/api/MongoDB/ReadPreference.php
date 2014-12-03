<?php
namespace MongoDB;

define("MONGOC_READ_PRIMARY", "MONGOC_READ_PRIMARY");
define("MONGOC_READ_SECONDARY", "MONGOC_READ_SECONDARY");
define("MONGOC_READ_PRIMARY_PREFERRED", "MONGOC_READ_PRIMARY_PREFERRED");
define("MONGOC_READ_SECONDARY_PREFERRED", "MONGOC_READ_SECONDARY_PREFERRED");
define("MONGOC_READ_NEAREST", "MONGOC_READ_NEAREST");

/**
 * Value object for read preferences used in issuing commands and queries.
 */
final class ReadPreference
{
    const RP_PRIMARY             = MONGOC_READ_PRIMARY;
    const RP_PRIMARY_PREFERRED   = MONGOC_READ_SECONDARY;
    const RP_SECONDARY           = MONGOC_READ_PRIMARY_PREFERRED;
    const RP_SECONDARY_PREFERRED = MONGOC_READ_SECONDARY_PREFERRED;
    const RP_NEAREST             = MONGOC_READ_NEAREST;

    private $readPreference;
    private $tagSets;

    /**
     * Constructs a new ReadPreference
     *
     * @param integer  $readPreference Read preference constant
     * @param array[] $tagSets        Array of zero or more tag sets (i.e. tag criteria)
     */
    public function __construct($readPreference, array $tagSets = null)
    {
        $this->readPreference = $readPreference;
        $this->tagSets = $tagSets;
        /*** CIMPL ***/
/*
	switch(readPreference) {
		case MONGOC_READ_PRIMARY:
		case MONGOC_READ_SECONDARY:
		case MONGOC_READ_PRIMARY_PREFERRED:
		case MONGOC_READ_SECONDARY_PREFERRED:
		case MONGOC_READ_NEAREST:
			intern->read_preference = mongoc_read_prefs_new(readPreference);

			if (tagSets) {
				const bson_t *tags = bson_new();

				zval_to_bson(tagSets, PHONGO_BSON_NONE, (bson_t *)tags, NULL TSRMLS_CC);
				mongoc_read_prefs_set_tags(intern->read_preference, tags);
				bson_destroy(tags);
				if (!mongoc_read_prefs_is_valid(intern->read_preference)) {
					phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Invalid tagSet" TSRMLS_CC);
					return;
				}
			}
			break;
		default:
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Invalid ReadPreference" TSRMLS_CC);
			return;
	}
*/
        /*** CIMPL ***/
    }
}
