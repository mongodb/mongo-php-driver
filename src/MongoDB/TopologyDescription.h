/*
 * Copyright 2022-present MongoDB, Inc.
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

#ifndef PHONGO_TOPOLOGYDESCRIPTION_H
#define PHONGO_TOPOLOGYDESCRIPTION_H

#include "mongoc/mongoc.h"

#include <php.h>

/* Note: these constants are derived from _mongoc_topology_description_type,
 * since mongoc_topology_description_t is private. */
#define PHONGO_TOPOLOGY_UNKNOWN "Unknown"
#define PHONGO_TOPOLOGY_SINGLE "Single"
#define PHONGO_TOPOLOGY_SHARDED "Sharded"
#define PHONGO_TOPOLOGY_REPLICA_SET_NO_PRIMARY "ReplicaSetNoPrimary"
#define PHONGO_TOPOLOGY_REPLICA_SET_WITH_PRIMARY "ReplicaSetWithPrimary"
#define PHONGO_TOPOLOGY_LOAD_BALANCED "LoadBalanced"

void phongo_topologydescription_init(zval* return_value, mongoc_topology_description_t* topology_description);

#endif /* PHONGO_TOPOLOGYDESCRIPTION_H */
