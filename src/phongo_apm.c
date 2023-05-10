/*
 * Copyright 2021-present MongoDB, Inc.
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

#include "bson/bson.h"
#include "mongoc/mongoc.h"

#include <php.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>
#include <Zend/zend_operators.h>

#include "php_phongo.h"
#include "phongo_apm.h"
#include "phongo_error.h"

ZEND_EXTERN_MODULE_GLOBALS(mongodb)

/* Ensures that instances of @subscriber_ce in @from (those registered with a
 * Manager or globally) are added to the set @to. This is used to build the list
 * of subscribers notify for an event. */
static void phongo_apm_add_subscribers_to_notify(zend_class_entry* subscriber_ce, HashTable* from, HashTable* to)
{
	zval* subscriber;

	ZEND_HASH_FOREACH_VAL_IND(from, subscriber)
	{
		if (Z_TYPE_P(subscriber) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(subscriber), subscriber_ce)) {
			continue;
		}

		if (zend_hash_index_exists(to, Z_OBJ_HANDLE_P(subscriber))) {
			continue;
		}

		zend_hash_index_update(to, Z_OBJ_HANDLE_P(subscriber), subscriber);
		Z_ADDREF_P(subscriber);
	}
	ZEND_HASH_FOREACH_END();
}

/* Returns a newly allocated HashTable, which will contain all subscribers of a
 * certain type that should be notified for an event on the specified client. */
static HashTable* phongo_apm_get_subscribers_to_notify(zend_class_entry* subscriber_ce, mongoc_client_t* client)
{
	HashTable* subscribers = NULL;

	ALLOC_HASHTABLE(subscribers);
	zend_hash_init(subscribers, 0, NULL, ZVAL_PTR_DTOR, 0);

	if (MONGODB_G(subscribers)) {
		phongo_apm_add_subscribers_to_notify(subscriber_ce, MONGODB_G(subscribers), subscribers);
	}

	if (MONGODB_G(managers)) {
		php_phongo_manager_t* manager;

		ZEND_HASH_FOREACH_PTR(MONGODB_G(managers), manager)
		{
			if (manager->client == client && manager->subscribers) {
				phongo_apm_add_subscribers_to_notify(subscriber_ce, manager->subscribers, subscribers);
			}
		}
		ZEND_HASH_FOREACH_END();
	}

	return subscribers;
}

/* Search for a Manager associated with the given client in the request-scoped
 * registry. If any Manager is found, copy it to @out, increment its ref-count,
 * and return true; otherwise, set @out to undefined and return false. */
static bool phongo_apm_copy_manager_for_client(mongoc_client_t* client, zval* out)
{
	php_phongo_manager_t* manager;

	ZVAL_UNDEF(out);

	if (!MONGODB_G(managers) || zend_hash_num_elements(MONGODB_G(managers)) == 0) {
		return false;
	}

	ZEND_HASH_FOREACH_PTR(MONGODB_G(managers), manager)
	{
		if (manager->client == client) {
			ZVAL_OBJ(out, &manager->std);
			Z_ADDREF_P(out);

			return true;
		}
	}
	ZEND_HASH_FOREACH_END();

	return false;
}

/* Dispatch an event to all subscribers in a HashTable. The caller is
 * responsible for ensuring that subscribers implement the correct interface. */
static void phongo_apm_dispatch_event(HashTable* subscribers, const char* function_name, zval* event)
{
	zval* subscriber;

	ZEND_HASH_FOREACH_VAL_IND(subscribers, subscriber)
	{
		if (EG(exception)) {
			break;
		}

		/* We can't use the zend_call_method_with_1_params macro here, as it
		 * assumes the function name is a string literal. */
		zend_call_method(PHONGO_COMPAT_OBJ_P(subscriber), NULL, NULL, function_name, strlen(function_name), NULL, 1, event, NULL);
	}
	ZEND_HASH_FOREACH_END();
}

static void phongo_apm_command_started(const mongoc_apm_command_started_t* event)
{
	mongoc_client_t*                  client;
	HashTable*                        subscribers;
	php_phongo_commandstartedevent_t* p_event;
	zval                              z_event;

	client      = mongoc_apm_command_started_get_context(event);
	subscribers = phongo_apm_get_subscribers_to_notify(php_phongo_commandsubscriber_ce, client);

	/* Return early if there are no APM subscribers to notify */
	if (zend_hash_num_elements(subscribers) == 0) {
		goto cleanup;
	}

	object_init_ex(&z_event, php_phongo_commandstartedevent_ce);
	p_event = Z_COMMANDSTARTEDEVENT_OBJ_P(&z_event);

	p_event->command_name         = estrdup(mongoc_apm_command_started_get_command_name(event));
	p_event->server_id            = mongoc_apm_command_started_get_server_id(event);
	p_event->operation_id         = mongoc_apm_command_started_get_operation_id(event);
	p_event->request_id           = mongoc_apm_command_started_get_request_id(event);
	p_event->command              = bson_copy(mongoc_apm_command_started_get_command(event));
	p_event->database_name        = estrdup(mongoc_apm_command_started_get_database_name(event));
	p_event->server_connection_id = mongoc_apm_command_started_get_server_connection_id_int64(event);
	p_event->has_service_id       = mongoc_apm_command_started_get_service_id(event) != NULL;

	if (p_event->has_service_id) {
		bson_oid_copy(mongoc_apm_command_started_get_service_id(event), &p_event->service_id);
	}

	if (!phongo_apm_copy_manager_for_client(client, &p_event->manager)) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Found no Manager for client in APM event context");
		zval_ptr_dtor(&z_event);

		goto cleanup;
	}

	phongo_apm_dispatch_event(subscribers, "commandStarted", &z_event);
	zval_ptr_dtor(&z_event);

cleanup:
	zend_hash_destroy(subscribers);
	FREE_HASHTABLE(subscribers);
}

static void phongo_apm_command_succeeded(const mongoc_apm_command_succeeded_t* event)
{
	mongoc_client_t*                    client;
	HashTable*                          subscribers;
	php_phongo_commandsucceededevent_t* p_event;
	zval                                z_event;

	client      = mongoc_apm_command_succeeded_get_context(event);
	subscribers = phongo_apm_get_subscribers_to_notify(php_phongo_commandsubscriber_ce, client);

	/* Return early if there are no APM subscribers to notify */
	if (zend_hash_num_elements(subscribers) == 0) {
		goto cleanup;
	}

	object_init_ex(&z_event, php_phongo_commandsucceededevent_ce);
	p_event = Z_COMMANDSUCCEEDEDEVENT_OBJ_P(&z_event);

	p_event->command_name         = estrdup(mongoc_apm_command_succeeded_get_command_name(event));
	p_event->server_id            = mongoc_apm_command_succeeded_get_server_id(event);
	p_event->operation_id         = mongoc_apm_command_succeeded_get_operation_id(event);
	p_event->request_id           = mongoc_apm_command_succeeded_get_request_id(event);
	p_event->duration_micros      = mongoc_apm_command_succeeded_get_duration(event);
	p_event->reply                = bson_copy(mongoc_apm_command_succeeded_get_reply(event));
	p_event->server_connection_id = mongoc_apm_command_succeeded_get_server_connection_id_int64(event);
	p_event->has_service_id       = mongoc_apm_command_succeeded_get_service_id(event) != NULL;

	if (p_event->has_service_id) {
		bson_oid_copy(mongoc_apm_command_succeeded_get_service_id(event), &p_event->service_id);
	}

	if (!phongo_apm_copy_manager_for_client(client, &p_event->manager)) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Found no Manager for client in APM event context");
		zval_ptr_dtor(&z_event);

		goto cleanup;
	}

	phongo_apm_dispatch_event(subscribers, "commandSucceeded", &z_event);
	zval_ptr_dtor(&z_event);

cleanup:
	zend_hash_destroy(subscribers);
	FREE_HASHTABLE(subscribers);
}

static void phongo_apm_command_failed(const mongoc_apm_command_failed_t* event)
{
	mongoc_client_t*                 client;
	HashTable*                       subscribers;
	php_phongo_commandfailedevent_t* p_event;
	zval                             z_event;
	bson_error_t                     tmp_error = { 0 };

	client      = mongoc_apm_command_failed_get_context(event);
	subscribers = phongo_apm_get_subscribers_to_notify(php_phongo_commandsubscriber_ce, client);

	/* Return early if there are no APM subscribers to notify */
	if (zend_hash_num_elements(subscribers) == 0) {
		goto cleanup;
	}

	object_init_ex(&z_event, php_phongo_commandfailedevent_ce);
	p_event = Z_COMMANDFAILEDEVENT_OBJ_P(&z_event);

	p_event->command_name         = estrdup(mongoc_apm_command_failed_get_command_name(event));
	p_event->server_id            = mongoc_apm_command_failed_get_server_id(event);
	p_event->operation_id         = mongoc_apm_command_failed_get_operation_id(event);
	p_event->request_id           = mongoc_apm_command_failed_get_request_id(event);
	p_event->duration_micros      = mongoc_apm_command_failed_get_duration(event);
	p_event->reply                = bson_copy(mongoc_apm_command_failed_get_reply(event));
	p_event->server_connection_id = mongoc_apm_command_failed_get_server_connection_id_int64(event);
	p_event->has_service_id       = mongoc_apm_command_failed_get_service_id(event) != NULL;

	if (p_event->has_service_id) {
		bson_oid_copy(mongoc_apm_command_failed_get_service_id(event), &p_event->service_id);
	}

	if (!phongo_apm_copy_manager_for_client(client, &p_event->manager)) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Found no Manager for client in APM event context");
		zval_ptr_dtor(&z_event);

		goto cleanup;
	}

	/* We need to process and convert the error right here, otherwise
	 * debug_info will turn into a recursive loop, and with the wrong trace
	 * locations */
	mongoc_apm_command_failed_get_error(event, &tmp_error);

	object_init_ex(&p_event->z_error, phongo_exception_from_mongoc_domain(tmp_error.domain, tmp_error.code));
	zend_update_property_string(zend_ce_exception, PHONGO_COMPAT_OBJ_P(&p_event->z_error), ZEND_STRL("message"), tmp_error.message);
	zend_update_property_long(zend_ce_exception, PHONGO_COMPAT_OBJ_P(&p_event->z_error), ZEND_STRL("code"), tmp_error.code);

	phongo_apm_dispatch_event(subscribers, "commandFailed", &z_event);
	zval_ptr_dtor(&z_event);

cleanup:
	zend_hash_destroy(subscribers);
	FREE_HASHTABLE(subscribers);
}

static void phongo_apm_server_changed(const mongoc_apm_server_changed_t* event)
{
	mongoc_client_t*                 client;
	HashTable*                       subscribers;
	php_phongo_serverchangedevent_t* p_event;
	zval                             z_event;

	client      = mongoc_apm_server_changed_get_context(event);
	subscribers = phongo_apm_get_subscribers_to_notify(php_phongo_sdamsubscriber_ce, client);

	/* Return early if there are no APM subscribers to notify */
	if (zend_hash_num_elements(subscribers) == 0) {
		goto cleanup;
	}

	object_init_ex(&z_event, php_phongo_serverchangedevent_ce);
	p_event = Z_SERVERCHANGEDEVENT_OBJ_P(&z_event);

	memcpy(&p_event->host, mongoc_apm_server_changed_get_host(event), sizeof(mongoc_host_list_t));
	mongoc_apm_server_changed_get_topology_id(event, &p_event->topology_id);
	p_event->new_server_description = mongoc_server_description_new_copy(mongoc_apm_server_changed_get_new_description(event));
	p_event->old_server_description = mongoc_server_description_new_copy(mongoc_apm_server_changed_get_previous_description(event));

	phongo_apm_dispatch_event(subscribers, "serverChanged", &z_event);
	zval_ptr_dtor(&z_event);

cleanup:
	zend_hash_destroy(subscribers);
	FREE_HASHTABLE(subscribers);
}

static void phongo_apm_server_closed(const mongoc_apm_server_closed_t* event)
{
	mongoc_client_t*                client;
	HashTable*                      subscribers;
	php_phongo_serverclosedevent_t* p_event;
	zval                            z_event;

	client      = mongoc_apm_server_closed_get_context(event);
	subscribers = phongo_apm_get_subscribers_to_notify(php_phongo_sdamsubscriber_ce, client);

	/* Return early if there are no APM subscribers to notify */
	if (zend_hash_num_elements(subscribers) == 0) {
		goto cleanup;
	}

	object_init_ex(&z_event, php_phongo_serverclosedevent_ce);
	p_event = Z_SERVERCLOSEDEVENT_OBJ_P(&z_event);

	memcpy(&p_event->host, mongoc_apm_server_closed_get_host(event), sizeof(mongoc_host_list_t));
	mongoc_apm_server_closed_get_topology_id(event, &p_event->topology_id);

	phongo_apm_dispatch_event(subscribers, "serverClosed", &z_event);
	zval_ptr_dtor(&z_event);

cleanup:
	zend_hash_destroy(subscribers);
	FREE_HASHTABLE(subscribers);
}

static void phongo_apm_server_heartbeat_failed(const mongoc_apm_server_heartbeat_failed_t* event)
{
	mongoc_client_t*                         client;
	HashTable*                               subscribers;
	php_phongo_serverheartbeatfailedevent_t* p_event;
	zval                                     z_event;
	bson_error_t                             tmp_error = { 0 };

	client      = mongoc_apm_server_heartbeat_failed_get_context(event);
	subscribers = phongo_apm_get_subscribers_to_notify(php_phongo_sdamsubscriber_ce, client);

	/* Return early if there are no APM subscribers to notify */
	if (zend_hash_num_elements(subscribers) == 0) {
		goto cleanup;
	}

	object_init_ex(&z_event, php_phongo_serverheartbeatfailedevent_ce);
	p_event = Z_SERVERHEARTBEATFAILEDEVENT_OBJ_P(&z_event);

	memcpy(&p_event->host, mongoc_apm_server_heartbeat_failed_get_host(event), sizeof(mongoc_host_list_t));
	p_event->awaited         = mongoc_apm_server_heartbeat_failed_get_awaited(event);
	p_event->duration_micros = mongoc_apm_server_heartbeat_failed_get_duration(event);

	/* We need to process and convert the error right here, otherwise
	 * debug_info will turn into a recursive loop, and with the wrong trace
	 * locations */
	mongoc_apm_server_heartbeat_failed_get_error(event, &tmp_error);

	object_init_ex(&p_event->z_error, phongo_exception_from_mongoc_domain(tmp_error.domain, tmp_error.code));
	zend_update_property_string(zend_ce_exception, PHONGO_COMPAT_OBJ_P(&p_event->z_error), ZEND_STRL("message"), tmp_error.message);
	zend_update_property_long(zend_ce_exception, PHONGO_COMPAT_OBJ_P(&p_event->z_error), ZEND_STRL("code"), tmp_error.code);

	phongo_apm_dispatch_event(subscribers, "serverHeartbeatFailed", &z_event);
	zval_ptr_dtor(&z_event);

cleanup:
	zend_hash_destroy(subscribers);
	FREE_HASHTABLE(subscribers);
}

static void phongo_apm_server_heartbeat_succeeded(const mongoc_apm_server_heartbeat_succeeded_t* event)
{
	mongoc_client_t*                            client;
	HashTable*                                  subscribers;
	php_phongo_serverheartbeatsucceededevent_t* p_event;
	zval                                        z_event;

	client      = mongoc_apm_server_heartbeat_succeeded_get_context(event);
	subscribers = phongo_apm_get_subscribers_to_notify(php_phongo_sdamsubscriber_ce, client);

	/* Return early if there are no APM subscribers to notify */
	if (zend_hash_num_elements(subscribers) == 0) {
		goto cleanup;
	}

	object_init_ex(&z_event, php_phongo_serverheartbeatsucceededevent_ce);
	p_event = Z_SERVERHEARTBEATSUCCEEDEDEVENT_OBJ_P(&z_event);

	memcpy(&p_event->host, mongoc_apm_server_heartbeat_succeeded_get_host(event), sizeof(mongoc_host_list_t));
	p_event->awaited         = mongoc_apm_server_heartbeat_succeeded_get_awaited(event);
	p_event->duration_micros = mongoc_apm_server_heartbeat_succeeded_get_duration(event);
	p_event->reply           = bson_copy(mongoc_apm_server_heartbeat_succeeded_get_reply(event));

	phongo_apm_dispatch_event(subscribers, "serverHeartbeatSucceeded", &z_event);
	zval_ptr_dtor(&z_event);

cleanup:
	zend_hash_destroy(subscribers);
	FREE_HASHTABLE(subscribers);
}

static void phongo_apm_server_heartbeat_started(const mongoc_apm_server_heartbeat_started_t* event)
{
	mongoc_client_t*                          client;
	HashTable*                                subscribers;
	php_phongo_serverheartbeatstartedevent_t* p_event;
	zval                                      z_event;

	client      = mongoc_apm_server_heartbeat_started_get_context(event);
	subscribers = phongo_apm_get_subscribers_to_notify(php_phongo_sdamsubscriber_ce, client);

	/* Return early if there are no APM subscribers to notify */
	if (zend_hash_num_elements(subscribers) == 0) {
		goto cleanup;
	}

	object_init_ex(&z_event, php_phongo_serverheartbeatstartedevent_ce);
	p_event = Z_SERVERHEARTBEATSTARTEDEVENT_OBJ_P(&z_event);

	memcpy(&p_event->host, mongoc_apm_server_heartbeat_started_get_host(event), sizeof(mongoc_host_list_t));
	p_event->awaited = mongoc_apm_server_heartbeat_started_get_awaited(event);

	phongo_apm_dispatch_event(subscribers, "serverHeartbeatStarted", &z_event);
	zval_ptr_dtor(&z_event);

cleanup:
	zend_hash_destroy(subscribers);
	FREE_HASHTABLE(subscribers);
}

static void phongo_apm_server_opening(const mongoc_apm_server_opening_t* event)
{
	mongoc_client_t*                 client;
	HashTable*                       subscribers;
	php_phongo_serveropeningevent_t* p_event;
	zval                             z_event;

	client      = mongoc_apm_server_opening_get_context(event);
	subscribers = phongo_apm_get_subscribers_to_notify(php_phongo_sdamsubscriber_ce, client);

	/* Return early if there are no APM subscribers to notify */
	if (zend_hash_num_elements(subscribers) == 0) {
		goto cleanup;
	}

	object_init_ex(&z_event, php_phongo_serveropeningevent_ce);
	p_event = Z_SERVEROPENINGEVENT_OBJ_P(&z_event);

	memcpy(&p_event->host, mongoc_apm_server_opening_get_host(event), sizeof(mongoc_host_list_t));
	mongoc_apm_server_opening_get_topology_id(event, &p_event->topology_id);

	phongo_apm_dispatch_event(subscribers, "serverOpening", &z_event);
	zval_ptr_dtor(&z_event);

cleanup:
	zend_hash_destroy(subscribers);
	FREE_HASHTABLE(subscribers);
}

static void phongo_apm_topology_changed(const mongoc_apm_topology_changed_t* event)
{
	mongoc_client_t*                   client;
	HashTable*                         subscribers;
	php_phongo_topologychangedevent_t* p_event;
	zval                               z_event;

	client      = mongoc_apm_topology_changed_get_context(event);
	subscribers = phongo_apm_get_subscribers_to_notify(php_phongo_sdamsubscriber_ce, client);

	/* Return early if there are no APM subscribers to notify */
	if (zend_hash_num_elements(subscribers) == 0) {
		goto cleanup;
	}

	object_init_ex(&z_event, php_phongo_topologychangedevent_ce);
	p_event = Z_TOPOLOGYCHANGEDEVENT_OBJ_P(&z_event);

	mongoc_apm_topology_changed_get_topology_id(event, &p_event->topology_id);
	p_event->new_topology_description = mongoc_topology_description_new_copy(mongoc_apm_topology_changed_get_new_description(event));
	p_event->old_topology_description = mongoc_topology_description_new_copy(mongoc_apm_topology_changed_get_previous_description(event));

	phongo_apm_dispatch_event(subscribers, "topologyChanged", &z_event);
	zval_ptr_dtor(&z_event);

cleanup:
	zend_hash_destroy(subscribers);
	FREE_HASHTABLE(subscribers);
}

static void phongo_apm_topology_closed(const mongoc_apm_topology_closed_t* event)
{
	mongoc_client_t*                  client;
	HashTable*                        subscribers;
	php_phongo_topologyclosedevent_t* p_event;
	zval                              z_event;

	client      = mongoc_apm_topology_closed_get_context(event);
	subscribers = phongo_apm_get_subscribers_to_notify(php_phongo_sdamsubscriber_ce, client);

	/* Return early if there are no APM subscribers to notify */
	if (zend_hash_num_elements(subscribers) == 0) {
		goto cleanup;
	}

	object_init_ex(&z_event, php_phongo_topologyclosedevent_ce);
	p_event = Z_TOPOLOGYCLOSEDEVENT_OBJ_P(&z_event);

	mongoc_apm_topology_closed_get_topology_id(event, &p_event->topology_id);

	phongo_apm_dispatch_event(subscribers, "topologyClosed", &z_event);
	zval_ptr_dtor(&z_event);

cleanup:
	zend_hash_destroy(subscribers);
	FREE_HASHTABLE(subscribers);
}

static void phongo_apm_topology_opening(const mongoc_apm_topology_opening_t* event)
{
	mongoc_client_t*                   client;
	HashTable*                         subscribers;
	php_phongo_topologyopeningevent_t* p_event;
	zval                               z_event;

	client      = mongoc_apm_topology_opening_get_context(event);
	subscribers = phongo_apm_get_subscribers_to_notify(php_phongo_sdamsubscriber_ce, client);

	/* Return early if there are no APM subscribers to notify */
	if (zend_hash_num_elements(subscribers) == 0) {
		goto cleanup;
	}

	object_init_ex(&z_event, php_phongo_topologyopeningevent_ce);
	p_event = Z_TOPOLOGYOPENINGEVENT_OBJ_P(&z_event);

	mongoc_apm_topology_opening_get_topology_id(event, &p_event->topology_id);

	phongo_apm_dispatch_event(subscribers, "topologyOpening", &z_event);
	zval_ptr_dtor(&z_event);

cleanup:
	zend_hash_destroy(subscribers);
	FREE_HASHTABLE(subscribers);
}

/* Assigns APM callbacks to a client, which will notify any global or per-client
 * subscribers. This should be called for all clients created by the driver.
 * Returns true on success; otherwise, throws an exception and returns false. */
bool phongo_apm_set_callbacks(mongoc_client_t* client)
{
	bool retval;

	mongoc_apm_callbacks_t* callbacks = mongoc_apm_callbacks_new();

	mongoc_apm_set_command_started_cb(callbacks, phongo_apm_command_started);
	mongoc_apm_set_command_succeeded_cb(callbacks, phongo_apm_command_succeeded);
	mongoc_apm_set_command_failed_cb(callbacks, phongo_apm_command_failed);
	mongoc_apm_set_server_changed_cb(callbacks, phongo_apm_server_changed);
	mongoc_apm_set_server_closed_cb(callbacks, phongo_apm_server_closed);
	mongoc_apm_set_server_heartbeat_failed_cb(callbacks, phongo_apm_server_heartbeat_failed);
	mongoc_apm_set_server_heartbeat_succeeded_cb(callbacks, phongo_apm_server_heartbeat_succeeded);
	mongoc_apm_set_server_heartbeat_started_cb(callbacks, phongo_apm_server_heartbeat_started);
	mongoc_apm_set_server_opening_cb(callbacks, phongo_apm_server_opening);
	mongoc_apm_set_topology_changed_cb(callbacks, phongo_apm_topology_changed);
	mongoc_apm_set_topology_closed_cb(callbacks, phongo_apm_topology_closed);
	mongoc_apm_set_topology_opening_cb(callbacks, phongo_apm_topology_opening);

	retval = mongoc_client_set_apm_callbacks(client, callbacks, client);

	if (!retval) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Failed to set APM callbacks");
	}

	mongoc_apm_callbacks_destroy(callbacks);

	return retval;
}

/* Checks args for adding/removing a subscriber. Returns true on success;
 * otherwise, throws an exception and returns false. */
static bool phongo_apm_check_args_for_add_and_remove(HashTable* subscribers, zval* subscriber)
{
	if (!subscribers) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Subscriber HashTable is not initialized");
		return false;
	}

	if (!subscriber || Z_TYPE_P(subscriber) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(subscriber), php_phongo_subscriber_ce)) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Subscriber is not an instance of %s", ZSTR_VAL(php_phongo_subscriber_ce->name));
		return false;
	}

	return true;
}

/* Adds a subscriber to the HashTable (global or Manager). Returns true on
 * success (including NOP if already registered); otherwise, throws an exception
 * and returns false. */
bool phongo_apm_add_subscriber(HashTable* subscribers, zval* subscriber)
{
	if (!phongo_apm_check_args_for_add_and_remove(subscribers, subscriber)) {
		/* Exception should already have been thrown */
		return false;
	}

	/* NOP if the subscriber was already registered */
	if (zend_hash_index_exists(subscribers, Z_OBJ_HANDLE_P(subscriber))) {
		return true;
	}

	zend_hash_index_update(subscribers, Z_OBJ_HANDLE_P(subscriber), subscriber);
	Z_ADDREF_P(subscriber);

	return true;
}

/* Removes a subscriber from the HashTable (global or Manager). Returns true on
 * success (including NOP if never registered); otherwise, throws an exception
 * and returns false. */
bool phongo_apm_remove_subscriber(HashTable* subscribers, zval* subscriber)
{
	if (!phongo_apm_check_args_for_add_and_remove(subscribers, subscriber)) {
		/* Exception should already have been thrown */
		return false;
	}

	/* Note: HashTables should specify ZVAL_PTR_DTOR as their element destructor
	 * so there is no need to decrement the subscriber's reference count here.
	 * We also don't care about whether zend_hash_index_del returns SUCCESS or
	 * FAILURE, as removing an unregistered subscriber is a NOP. */
	zend_hash_index_del(subscribers, Z_OBJ_HANDLE_P(subscriber));

	return true;
}
