/*
 * Copyright 2014-present MongoDB, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef PHONGO_CLASSES_H
#define PHONGO_CLASSES_H

#include "phongo_structs.h"

/*
 * Helper macros for classes
 *
 * The PHONGO_DECLARE_CLASS and PHONGO_DECLARE_CLASS_WITH_HELPERS expand to all of the declarations necessary for a
 * complete PHP class.
 * - PHONGO_DECLARE_CLASS(foo) will generate external class entry variables and the corresponding init_ce function, e.g.
 *   phongo_foo_ce and phongo_foo_init_ce()
 * - PHONGO_DECLARE_CLASS_WITH_HELPERS(FOO, foo) generates the above, plus additional helpers for working with zvals:
 *   - Z_FOO_OBJ_P() returns a phongo_foo_t* from a given zval*
 *   - Z_OBJ_FOO() returns a phongo_foo_t* from a given zend_object*
 *
 * Helpers assume the presence of a phongo_foo_t struct in phongo_structs.h
 */

#define PHONGO_INTERN_FROM_ZVAL(name, z) phongo_##name##_t* intern = php_##name##_fetch_object(Z_OBJ_P(z))
#define PHONGO_INTERN_FROM_Z_OBJ(name, obj) phongo_##name##_t* intern = php_##name##_fetch_object((obj))
#define PHONGO_INTERN_FROM_THIS(name) PHONGO_INTERN_FROM_ZVAL(name, getThis())

#define PHONGO_INTERN_OBJECT_ALLOC(name, class_type)                       \
	phongo_##name##_t* intern;                                             \
	do {                                                                   \
		intern = zend_object_alloc(sizeof(phongo_##name##_t), class_type); \
		zend_object_std_init(&intern->std, class_type);                    \
		object_properties_init(&intern->std, class_type);                  \
	} while (0)

#define PHONGO_INTERN_INIT_EX(name, z)                  \
	phongo_##name##_t* intern;                          \
	do {                                                \
		object_init_ex(z, phongo_##name##_ce);          \
		intern = php_##name##_fetch_object(Z_OBJ_P(z)); \
	} while (0)

#define PHONGO_OBJECT_INIT_EX(name, z)         \
	zend_object* object;                       \
	do {                                       \
		object_init_ex(z, phongo_##name##_ce); \
		object = Z_OBJ_P(z);                   \
	} while (0)

#define PHONGO_RETURN_PROPERTY(name, prop)                                                                            \
	do {                                                                                                              \
		zval rv;                                                                                                      \
		RETURN_ZVAL(zend_read_property(phongo_##name##_ce, Z_OBJ_P(getThis()), ZEND_STRL((prop)), false, &rv), 1, 0); \
	} while (0);

#define PHONGO_PROPERTY_GETTER(className, getter, name, property) \
	static PHP_METHOD(className, getter)                          \
	{                                                             \
		PHONGO_PARSE_PARAMETERS_NONE();                           \
                                                                  \
		PHONGO_RETURN_PROPERTY(name, property);                   \
	}

#define CLASS_FETCH_OBJ_DECL(name)                                                      \
	static inline phongo_##name##_t* php_##name##_fetch_object(const zend_object* obj)  \
	{                                                                                   \
		return (phongo_##name##_t*) ((char*) obj - XtOffsetOf(phongo_##name##_t, std)); \
	}

#define CLASS_ENTRY_DECL(name) extern zend_class_entry* phongo_##name##_ce

#define CE_INIT_FUNC_DECL(name) extern void phongo_##name##_init_ce(INIT_FUNC_ARGS)

#define Z_OBJ_P_FETCH_DECL(ucname, name)                                \
	static inline phongo_##name##_t* Z_##ucname##_OBJ_P(const zval* zv) \
	{                                                                   \
		return php_##name##_fetch_object(Z_OBJ_P(zv));                  \
	}

#define Z_OBJ_DECL(ucname, name)                                           \
	static inline phongo_##name##_t* Z_OBJ_##ucname(const zend_object* zo) \
	{                                                                      \
		return php_##name##_fetch_object(zo);                              \
	}

#define PHONGO_DECLARE_CLASS(name) \
	CLASS_ENTRY_DECL(name);        \
	CE_INIT_FUNC_DECL(name);

#define PHONGO_DECLARE_CLASS_WITH_HELPERS(ucname, name) \
	PHONGO_DECLARE_CLASS(name)                          \
	CLASS_FETCH_OBJ_DECL(name)                          \
	Z_OBJ_P_FETCH_DECL(ucname, name)                    \
	Z_OBJ_DECL(ucname, name)

/* Export zend_class_entry dependencies, which are initialized in MINIT */
extern zend_class_entry* phongo_json_serializable_ce;

PHONGO_DECLARE_CLASS_WITH_HELPERS(CLIENTENCRYPTION, clientencryption)
PHONGO_DECLARE_CLASS_WITH_HELPERS(COMMAND, command)
PHONGO_DECLARE_CLASS_WITH_HELPERS(CURSOR, cursor)
PHONGO_DECLARE_CLASS_WITH_HELPERS(MANAGER, manager)
PHONGO_DECLARE_CLASS_WITH_HELPERS(QUERY, query)
PHONGO_DECLARE_CLASS_WITH_HELPERS(READCONCERN, readconcern)
PHONGO_DECLARE_CLASS_WITH_HELPERS(READPREFERENCE, readpreference)
PHONGO_DECLARE_CLASS_WITH_HELPERS(SERVER, server)
PHONGO_DECLARE_CLASS_WITH_HELPERS(SERVERAPI, serverapi)
PHONGO_DECLARE_CLASS_WITH_HELPERS(SERVERDESCRIPTION, serverdescription)
PHONGO_DECLARE_CLASS_WITH_HELPERS(SESSION, session)
PHONGO_DECLARE_CLASS_WITH_HELPERS(TOPOLOGYDESCRIPTION, topologydescription)
PHONGO_DECLARE_CLASS_WITH_HELPERS(BULKWRITE, bulkwrite)
PHONGO_DECLARE_CLASS_WITH_HELPERS(BULKWRITECOMMAND, bulkwritecommand)
PHONGO_DECLARE_CLASS_WITH_HELPERS(BULKWRITECOMMANDRESULT, bulkwritecommandresult)
PHONGO_DECLARE_CLASS_WITH_HELPERS(WRITECONCERN, writeconcern)
PHONGO_DECLARE_CLASS_WITH_HELPERS(WRITERESULT, writeresult)
PHONGO_DECLARE_CLASS_WITH_HELPERS(BINARY, binary)
PHONGO_DECLARE_CLASS_WITH_HELPERS(DOCUMENT, document)
PHONGO_DECLARE_CLASS_WITH_HELPERS(ITERATOR, iterator)
PHONGO_DECLARE_CLASS_WITH_HELPERS(DBPOINTER, dbpointer)
PHONGO_DECLARE_CLASS_WITH_HELPERS(DECIMAL128, decimal128)
PHONGO_DECLARE_CLASS_WITH_HELPERS(INT64, int64)
PHONGO_DECLARE_CLASS_WITH_HELPERS(JAVASCRIPT, javascript)
PHONGO_DECLARE_CLASS_WITH_HELPERS(OBJECTID, objectid)
PHONGO_DECLARE_CLASS_WITH_HELPERS(PACKEDARRAY, packedarray)
PHONGO_DECLARE_CLASS_WITH_HELPERS(SYMBOL, symbol)
PHONGO_DECLARE_CLASS_WITH_HELPERS(TIMESTAMP, timestamp)
PHONGO_DECLARE_CLASS_WITH_HELPERS(UNDEFINED, undefined)
PHONGO_DECLARE_CLASS_WITH_HELPERS(UTCDATETIME, utcdatetime)
PHONGO_DECLARE_CLASS_WITH_HELPERS(SERVERCHANGEDEVENT, serverchangedevent)
PHONGO_DECLARE_CLASS_WITH_HELPERS(TOPOLOGYCHANGEDEVENT, topologychangedevent)

/*
 * On PHP < 8.4 the preprocessor version used for windows builds can cause issues when replacing ##name## macro tokens
 * if the name is an identifier (e.g. ##REGEX##). To work around this, we declare the class without helpers, then
 * manually define the Z_REGEX_OBJ_P and Z_OBJ_REGEX helpers.
 * This can be replaced with a single PHONGO_DECLARE_CLASS_WITH_HELPERS call when dropping support for PHP 8.3
 */
PHONGO_DECLARE_CLASS(regex)
CLASS_FETCH_OBJ_DECL(regex)
static inline phongo_regex_t* Z_REGEX_OBJ_P(const zval* zv)
{
	return php_regex_fetch_object(Z_OBJ_P(zv));
}
static inline phongo_regex_t* Z_OBJ_REGEX(const zend_object* zo)
{
	return php_regex_fetch_object(zo);
}

PHONGO_DECLARE_CLASS(writeerror)
PHONGO_DECLARE_CLASS(writeconcernerror)

PHONGO_DECLARE_CLASS(cursor_interface)

PHONGO_DECLARE_CLASS(exception)
PHONGO_DECLARE_CLASS(logicexception)
PHONGO_DECLARE_CLASS(runtimeexception)
PHONGO_DECLARE_CLASS(serverexception)
PHONGO_DECLARE_CLASS(commandexception)
PHONGO_DECLARE_CLASS(unexpectedvalueexception)
PHONGO_DECLARE_CLASS(invalidargumentexception)
PHONGO_DECLARE_CLASS(connectionexception)
PHONGO_DECLARE_CLASS(authenticationexception)
PHONGO_DECLARE_CLASS(encryptionexception)
PHONGO_DECLARE_CLASS(executiontimeoutexception)
PHONGO_DECLARE_CLASS(connectiontimeoutexception)
PHONGO_DECLARE_CLASS(bulkwriteexception)
PHONGO_DECLARE_CLASS(bulkwritecommandexception)

PHONGO_DECLARE_CLASS(type)
PHONGO_DECLARE_CLASS(persistable)
PHONGO_DECLARE_CLASS(unserializable)
PHONGO_DECLARE_CLASS(serializable)
PHONGO_DECLARE_CLASS(vectortype)

PHONGO_DECLARE_CLASS(maxkey)
PHONGO_DECLARE_CLASS(minkey)

PHONGO_DECLARE_CLASS(binary_interface)
PHONGO_DECLARE_CLASS(decimal128_interface)
PHONGO_DECLARE_CLASS(javascript_interface)
PHONGO_DECLARE_CLASS(maxkey_interface)
PHONGO_DECLARE_CLASS(minkey_interface)
PHONGO_DECLARE_CLASS(objectid_interface)
PHONGO_DECLARE_CLASS(regex_interface)
PHONGO_DECLARE_CLASS(timestamp_interface)
PHONGO_DECLARE_CLASS(utcdatetime_interface)

PHONGO_DECLARE_CLASS(subscriber)
PHONGO_DECLARE_CLASS(commandsubscriber)
PHONGO_DECLARE_CLASS(logsubscriber)
PHONGO_DECLARE_CLASS(sdamsubscriber)

PHONGO_DECLARE_CLASS(commandstartedevent)
PHONGO_DECLARE_CLASS(commandfailedevent)
PHONGO_DECLARE_CLASS(commandsucceededevent)
PHONGO_DECLARE_CLASS(serverclosedevent)
PHONGO_DECLARE_CLASS(serverheartbeatfailedevent)
PHONGO_DECLARE_CLASS(serverheartbeatstartedevent)
PHONGO_DECLARE_CLASS(serverheartbeatsucceededevent)
PHONGO_DECLARE_CLASS(serveropeningevent)
PHONGO_DECLARE_CLASS(topologyclosedevent)
PHONGO_DECLARE_CLASS(topologyopeningevent)

#undef CLASS_FETCH_OBJ_DECL
#undef CLASS_ENTRY_DECL
#undef CE_INIT_FUNC_DECL
#undef Z_OBJ_P_FETCH_DECL
#undef Z_OBJ_DECL
#undef PHONGO_DECLARE_CLASS
#undef PHONGO_DECLARE_CLASS_WITH_HELPERS

#endif /* PHONGO_CLASSES_H */
