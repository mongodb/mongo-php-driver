#ifndef PHONGO_COMPAT_MONGOCRYPT_H
#define PHONGO_COMPAT_MONGOCRYPT_H

/* This header forwards includes for "mongocrypt/mongocrypt.h" to the desired
 * header in the bundled libmongocrypt submodule. This is used when compiling
 * bundled versions of libmongoc and libmongocrypt and is necessary because
 * libmongocrypt's source directory is not named "mongocrypt/".
 *
 * This path is relative to the PHPC project directory to avoid ambiguity. */
#include "src/libmongocrypt/src/mongocrypt.h"

#endif /* PHONGO_COMPAT_MONGOCRYPT_H */
