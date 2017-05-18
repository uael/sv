/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org>
 */

#ifndef SEMVER_H__
# define SEMVER_H__

#include <stddef.h>

#ifndef SEMVER_COMPILE
# define SEMVER_COMPILE (0)
#endif

#ifndef BUILD_DYNAMIC_LINK
# define BUILD_DYNAMIC_LINK (0)
#endif

#if BUILD_DYNAMIC_LINK && defined(_MSC_VER)
# define EXPORT_LINK __declspec(dllexport)
# define IMPORT_LINK __declspec(dllimport)
#else
# define EXPORT_LINK
# define IMPORT_LINK
#endif
#if SEMVER_COMPILE
# ifdef __cplusplus
#   define SEMVER_API extern "C" EXPORT_LINK
# else
#   define SEMVER_API extern EXPORT_LINK
# endif
#else
# ifdef __cplusplus
#   define SEMVER_API extern "C" IMPORT_LINK
# else
#   define SEMVER_API extern IMPORT_LINK
# endif
#endif

#if !defined(__cplusplus) && defined(_MSC_VER) && _MSC_VER < 1900
# define bool	unsigned char
# define true	1
# define false	0
# define __bool_true_false_are_defined	1
#else
# ifdef __cplusplus
#  include <cstdbool>
# else
#  include <stdbool.h>
# endif
#endif

#ifndef semver_malloc
# define semver_malloc malloc
#endif

#ifndef semver_free
# define semver_free free
#endif

#define SEMVER_NUM_X (-1)

typedef struct semver semver_t;
typedef struct semver_id semver_id_t;
typedef struct semver_comp semver_comp_t;
typedef struct semver_range semver_range_t;

enum semver_op {
  SEMVER_OP_EQ = 0,
  SEMVER_OP_LT,
  SEMVER_OP_LE,
  SEMVER_OP_GT,
  SEMVER_OP_GE,
};

SEMVER_API char semver_num_read(int *self, const char *str, size_t len, size_t *offset);
SEMVER_API char semver_num_comp(int self, int other);

struct semver_id {
  bool numeric;
  int num;
  size_t len;
  const char *raw;
  struct semver_id *next;
};

SEMVER_API void semver_id_ctor(semver_id_t *self);
SEMVER_API void semver_id_dtor(semver_id_t *self);
SEMVER_API char semver_id_read(semver_id_t *self, const char *str, size_t len, size_t *offset);
SEMVER_API int  semver_id_pwrite(const semver_id_t *self, char *buffer, size_t len);
SEMVER_API char semver_id_pcomp(const semver_id_t *self, const semver_id_t *other);

#define semver_id_write(self, buffer, len) semver_id_pwrite(&(self), buffer, len)
#define semver_id_comp(self, other) semver_id_pcomp(&(self), &(other))

struct semver {
  int major, minor, patch;
  semver_id_t prerelease, build;
  size_t len;
  const char *raw;
};

SEMVER_API void semver_ctor(semver_t *self);
SEMVER_API void semver_dtor(semver_t *self);
SEMVER_API char semver_read(semver_t *self, const char *str, size_t len, size_t *offset);
SEMVER_API int  semver_pwrite(const semver_t *self, char *buffer, size_t len);
SEMVER_API char semver_pcomp(const semver_t *self, const semver_t *other);

#define semver_write(self, buffer, len) semver_pwrite(&(self), buffer, len)
#define semver_comp(self, other) semver_pcomp(&(self), &(other))

struct semver_comp {
  struct semver_comp *next;
  enum semver_op op;
  semver_t version;
};

SEMVER_API void semver_comp_ctor(semver_comp_t *self);
SEMVER_API void semver_comp_dtor(semver_comp_t *self);
SEMVER_API char semver_comp_read(semver_comp_t *self, const char *str, size_t len, size_t *offset);
SEMVER_API int  semver_comp_pwrite(const semver_comp_t *self, char *buffer, size_t len);
SEMVER_API char semver_pmatch(const semver_t *self, const semver_comp_t *comp);

#define semver_comp_write(self, buffer, len) semver_comp_pwrite(&(self), buffer, len)
#define semver_match(self, comp) semver_pmatch(&(self), &(comp))

struct semver_range {
  struct semver_range *next;
  semver_comp_t comp;
};

SEMVER_API void semver_range_dtor(semver_range_t *self);
SEMVER_API char semver_range_read(semver_range_t *self, const char *str, size_t len, size_t *offset);
SEMVER_API int  semver_range_pwrite(const semver_range_t *self, char *buffer, size_t len);
SEMVER_API char semver_prmatch(const semver_t *self, const semver_range_t *range);

#define semver_range_write(self, buffer, len) semver_range_pwrite(&(self), buffer, len)
#define semver_rmatch(self, comp) semver_prmatch(&(self), &(comp))

#endif /* SEMVER_H__ */
